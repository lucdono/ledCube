/******************************************************************************
 * Includes
 ******************************************************************************/
#include "clock_config.h"
#include "board.h"
#include "pin_mux.h"
#include "fsl_adc.h"
#include "fsl_spi.h"
#include "fsl_gpio.h"
#include "fsl_pint.h"
#include "fsl_syscon.h"
#include "fsl_power.h"
#include "LPC824.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "semphr.h"

#include "cube_interface.h"

/******************************************************************************
 * Defines - PIN <-> Cube Connections
 ******************************************************************************/
/*
 * SPI0 -> Shift Registers:
 *   - SCK              -->  PIN0_14
 *   - MISO             -->  PIN0_12
 *   - MOSI             -->  PIN0_13
 *   - CS               -->  PIN0_15
 *
 * GPIO0 - > Decoder 3 to 8:
 *   - DEC_BIT0 [A]     -->  PIN0_25
 *   - DEC_BIT1 [B]     -->  PIN0_26
 *   - DEC_BIT2 [C]     -->  PIN0_27
 *   - DEC_EN   [D]     -->  PIN0_28
 *
 */
#define PLANE_SPI_SCK			14U
#define PLANE_SPI_MOSI			12U
#define PLANE_SPI_MISO			13U
#define PLANE_SPI_SSEL			15U

#define PLANE_DEC_BIT0			25U
#define PLANE_DEC_BIT1			26U
#define PLANE_DEC_BIT2			27U
#define PLANE_DEC_EN			28U

/******************************************************************************
 * Defines
 ******************************************************************************/
#define BOARD_PORT 				 0U
#define BOARD_LED_BLUE_PIN		17U
#define BOARD_LED_GREEN_PIN		16U

#define ADC_SAMPLE_CHANNEL 		 0U
#define ADC_CLOCK_DIVIDER 		 1U

#define MODE_ALL		 		 0U
#define MODE_FIXED		 		 1U
#define MODE_VOLUME		 		 2U

#define MODES			 		 3U

/******************************************************************************
 * Internal Variables
 ******************************************************************************/
static SemaphoreHandle_t mutex_v;
static uint16_t sample = 0;
static adc_result_info_t gAdcResultInfoStruct;
adc_result_info_t *volatile gAdcResultInfoPtr = &gAdcResultInfoStruct;
static uint16_t adc_samples[ADC_BUFFER_DEPTH];
static uint8_t mode = 0;

/******************************************************************************
 * Prototypes
 ******************************************************************************/
static void vTaskEffects(void *pvParameters);
static void vTaskRefresh(void *pvParameters);
static void vTaskADC(void *pvParameters);
static void ADC_Configuration(void);
static void SPI_Configuration(void);
static void GPIO_Configuration(void);
static void effect_switch_callback(pint_pin_int_t pintr, uint32_t pmatch_status);
static void effect_mode_callback(pint_pin_int_t pintr, uint32_t pmatch_status);

/******************************************************************************
 * Functions
 ******************************************************************************/

/*------------------------------------------------------------------------------
 Cube OS/IO APIs implementations
 -----------------------------------------------------------------------------*/

void ledQB_board_init(void) {
	GPIO_Configuration();
	SPI_Configuration();
	ADC_Configuration();
}

void ledQB_board_plane_select(uint8_t plane) {
	uint32_t mask = ledQB_board_plane_mask(plane,
			GPIO_PortRead(GPIO, BOARD_PORT),
			PLANE_DEC_BIT0, PLANE_DEC_BIT1, PLANE_DEC_BIT2, PLANE_DEC_EN);

	GPIO_PortMaskedWrite(GPIO, BOARD_PORT, mask);
}

void ledQB_board_plane_send(uint8_t *data, uint8_t size) {
	spi_transfer_t xfer = { 0 };

	xfer.txData = data;
	xfer.dataSize = size;
	xfer.configFlags = kSPI_EndOfTransfer | kSPI_ReceiveIgnore;
	SPI_MasterTransferBlocking(SPI0, &xfer);
}

void ledQB_osal_sleep(uint8_t sleep_ms) {
	vTaskDelay(sleep_ms / portTICK_PERIOD_MS);
}

uint32_t ledQB_osal_time_now(void) {
	return xTaskGetTickCount() * portTICK_PERIOD_MS;
}

void ledQB_osal_lock(void) {
	xSemaphoreTake(mutex_v, portMAX_DELAY);
}

void ledQB_osal_unlock(void) {
	xSemaphoreGive(mutex_v);
}

void ledQB_osal_init(void) {

	mutex_v = xSemaphoreCreateMutex();

	xTaskCreate(vTaskEffects, "vTaskEffects", configMINIMAL_STACK_SIZE, NULL,
			(tskIDLE_PRIORITY + 1UL), (TaskHandle_t *) NULL);

	xTaskCreate(vTaskRefresh, "vTaskRefresh", configMINIMAL_STACK_SIZE, NULL,
			(tskIDLE_PRIORITY + 1UL), (TaskHandle_t *) NULL);

	xTaskCreate(vTaskADC, "vTaskADC", configMINIMAL_STACK_SIZE,
			NULL, (tskIDLE_PRIORITY + 1UL), (TaskHandle_t *) NULL);
}

static void GPIO_Configuration(void) {
	SYSCON_AttachSignal(SYSCON, kPINT_PinInt0, kSYSCON_GpioPort0Pin1ToPintsel);
	PINT_PinInterruptConfig(PINT, kPINT_PinInt0, kPINT_PinIntEnableRiseEdge,
			effect_switch_callback);
	PINT_EnableCallbackByIndex(PINT, kPINT_PinInt0);

	SYSCON_AttachSignal(SYSCON, kPINT_PinInt1, kSYSCON_GpioPort0Pin6ToPintsel);
	PINT_PinInterruptConfig(PINT, kPINT_PinInt1, kPINT_PinIntEnableRiseEdge,
			effect_mode_callback);
	PINT_EnableCallbackByIndex(PINT, kPINT_PinInt1);
}

static void SPI_Configuration(void) {
	spi_master_config_t userConfig = { 0 };
	uint32_t srcFreq = 0U;

	SPI_MasterGetDefaultConfig(&userConfig);
	userConfig.baudRate_Bps = 500000U;
	userConfig.sselNumber = kSPI_Ssel0Assert;
	userConfig.clockPolarity = kSPI_ClockPolarityActiveLow;
	userConfig.direction = kSPI_MsbFirst;
	userConfig.clockPhase = kSPI_ClockPhaseSecondEdge;
	userConfig.dataWidth = kSPI_Data8Bits;
	userConfig.sselPolarity = kSPI_SpolActiveAllHigh;

	srcFreq = CLOCK_GetFreq(kCLOCK_MainClk);
	SPI_MasterInit(SPI0, &userConfig, srcFreq);
}

static void ADC_Configuration(void) {

	POWER_DisablePD(kPDRUNCFG_PD_ADC0);

	uint32_t frequency = CLOCK_GetFreq(kCLOCK_Irc);
	ADC_DoSelfCalibration(ADC0, frequency);

	adc_config_t adcConfigStruct;
	adc_conv_seq_config_t adcConvSeqConfigStruct;

	adcConfigStruct.clockDividerNumber = ADC_CLOCK_DIVIDER;
	adcConfigStruct.enableLowPowerMode = false;
	ADC_Init(ADC0, &adcConfigStruct);

	adcConvSeqConfigStruct.channelMask = (1U << ADC_SAMPLE_CHANNEL); /* Includes channel DEMO_ADC_SAMPLE_CHANNEL_NUMBER. */
	adcConvSeqConfigStruct.triggerMask = 0U;
	adcConvSeqConfigStruct.triggerPolarity = kADC_TriggerPolarityPositiveEdge;
	adcConvSeqConfigStruct.enableSingleStep = false;
	adcConvSeqConfigStruct.enableSyncBypass = false;
	adcConvSeqConfigStruct.interruptMode = kADC_InterruptForEachSequence;
	ADC_SetConvSeqAConfig(ADC0, &adcConvSeqConfigStruct);
	ADC_EnableConvSeqA(ADC0, true); /* Enable the conversion sequence A. */

	ADC_DoSoftwareTriggerConvSeqA(ADC0);
	while (!ADC_GetChannelConversionResult(ADC0, ADC_SAMPLE_CHANNEL,
			&gAdcResultInfoStruct)) {
	}
	ADC_GetConvSeqAGlobalConversionResult(ADC0, &gAdcResultInfoStruct);

	ADC_EnableInterrupts(ADC0, kADC_ConvSeqAInterruptEnable);
	NVIC_EnableIRQ(ADC0_SEQA_IRQn);
}

/*------------------------------------------------------------------------------
 Handlers
 -----------------------------------------------------------------------------*/

static void effect_switch_callback(pint_pin_int_t pintr, uint32_t pmatch_status) {
	ledQB_quit_effect();
}

static void effect_mode_callback(pint_pin_int_t pintr, uint32_t pmatch_status) {
	mode = (mode + 1) % MODES;

	switch (mode) {
	case MODE_ALL: {
		ledQB_set_runMode("");
		GPIO_PortClear(GPIO, BOARD_PORT, 1u << BOARD_LED_BLUE_PIN);
	}
		break;
	case MODE_FIXED: {
		char *effect = ledQB_get_currentEffect();
		ledQB_set_runMode(effect);
		GPIO_PortSet(GPIO, BOARD_PORT, 1u << BOARD_LED_BLUE_PIN);
	}
		break;
	case MODE_VOLUME: {
		ledQB_set_runMode("volume");
		GPIO_PortToggle(GPIO, BOARD_PORT, 1u << BOARD_LED_GREEN_PIN);
	}
		break;
	}
}

void ADC0_SEQA_IRQHandler(void) {
	if (kADC_ConvSeqAInterruptFlag
			== (kADC_ConvSeqAInterruptFlag & ADC_GetStatusFlags(ADC0))) {
		ADC_EnableConvSeqABurstMode(ADC0, false);

		ADC_GetChannelConversionResult(ADC0, ADC_SAMPLE_CHANNEL,
				gAdcResultInfoPtr);
		ADC_ClearStatusFlags(ADC0, kADC_ConvSeqAInterruptFlag);
		adc_samples[sample++ % ADC_BUFFER_DEPTH] = gAdcResultInfoStruct.result & 0xFFFF;
	}
}

/*------------------------------------------------------------------------------
 Application OS threads
 -----------------------------------------------------------------------------*/

static void vTaskEffects(void *pvParameters) {
	uint8_t i = 0;

	while (true) {
		if (ledQB_effects(i))
			i = 0;
		else
			i++;

		vTaskDelay(1);
	}
}

static void vTaskRefresh(void *pvParameters) {
	while (true) {
		ledQB_refresh();
		vTaskDelay(1);
	}
}

static void vTaskADC(void *pvParameters) {
	uint16_t n = 0;
	while (true) {
		for (n = 0; n < ADC_BUFFER_DEPTH; n++)
			ADC_EnableConvSeqABurstMode(ADC0, true);

		ledQB_adc_sample(adc_samples);
		vTaskDelay(1);
	}
}

/*------------------------------------------------------------------------------
 Application entry point
 -----------------------------------------------------------------------------*/
int main(void) {
	BOARD_InitBootPins();
	BOARD_InitBootClocks();

	ledQB_init();
	if (strlen(ledQB_get_runMode()) == 0)
		GPIO_PortSet(GPIO, BOARD_PORT, 1u << BOARD_LED_BLUE_PIN);
	else
		GPIO_PortClear(GPIO, BOARD_PORT, 1u << BOARD_LED_BLUE_PIN);

	vTaskStartScheduler();

	return 0;
}
