/*
 * 3D LED CUBE LPC824 Implementation
 *
 * Copyright (C) 2021 Luca D'Onofrio.
 *
 * This file is part of LEDCube Project
 *
 * LEDCube is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * LEDCube is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdio.h>

#include "clock_config.h"
#include "board.h"
#include "pin_mux.h"
#include "fsl_adc.h"
#include "fsl_spi.h"
#include "fsl_gpio.h"
#include "fsl_pint.h"
#include "fsl_syscon.h"
#include "fsl_power.h"
#include "fsl_i2c.h"

#include "ssd1306_oled.h"
#include "ssd1306_fonts.h"
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
 *   - MISO             -->  PIN0_13
 *   - MOSI             -->  PIN0_12
 *   - CS               -->  PIN0_15
 *
 * GPIO0 - > Decoder 3 to 8:
 *   - DEC_BIT0 [A]     -->  PIN0_25
 *   - DEC_BIT1 [B]     -->  PIN0_26
 *   - DEC_BIT2 [C]     -->  PIN0_27
 *   - DEC_EN   [D]     -->  PIN0_28
 *
 * GPIO0 - > User buttons:
 *   - Mode Selection   -->  PIN0_06
 *   - Effect skip      -->  PIN0_01
 *
 * GPIO0 - > Board LEDs:
 *   - Red   			-->  PIN0_15
 *   - Green      		-->  PIN0_16
 *   - Blue				-->	 PIN0_17
 *
 * ADC CH0 -> Mic:
 *   - ADC0      		-->  PIN0_07
 *
 * I2C0 -> Display:
 *   - SCL      		-->  PIN0_10
 *   - SDA      		-->  PIN0_11
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
#define BOARD_PORT 				0U
#define BOARD_LED_BLUE_PIN		17U
#define BOARD_LED_GREEN_PIN		16U

#define ADC_SAMPLE_CHANNEL 		0U
#define ADC_CLOCK_DIVIDER 		1U
#define ADC_DEFAULT_MAX 		150U
#define ADC_DEFAULT_MIN 		100U
#define ADC_DEFAULT_INCREMENT	(-10)

#define MODE_ALL		 		0U
#define MODE_FIXED		 		1U
#define MODE_VOLUME		 		2U
#define MODE_LIFE		 		3U
#define STR_MODE_LIFE 			"Conway's Life"
#define STR_MODE_VOLUME 		"Volume"
#define STR_MODE_FIXED 			"Fixed"
#define STR_MODE_ALL_EFFECTS	"All Effects"
#define STR_STATS 				"Stats:"
#define STR_MODE 				"Mode:"
#define STR_EFFECT 				"Effect: "
#define MODES			 		4U
#define SCORE_SIZE				15

#define I2C_MASTER 				((I2C_Type *)I2C0_BASE)

/******************************************************************************
 * Internal Variables
 ******************************************************************************/
static SemaphoreHandle_t mutex_v;
static adc_result_info_t gAdcResultInfoStruct;
adc_result_info_t *volatile gAdcResultInfoPtr = &gAdcResultInfoStruct;
static uint16_t adc_samples[ADC_BUFFER_DEPTH];
static uint8_t mode = 0;
static char score[SCORE_SIZE];
volatile bool gAdcConvSeqAIntFlag;
static uint16_t max_sensivity = ADC_DEFAULT_MAX;

/******************************************************************************
 * External Variables
 ******************************************************************************/
extern uint16_t max_population;
extern uint16_t max_generations;

/******************************************************************************
 * Prototypes
 ******************************************************************************/
static void vTaskEffects(void *pvParameters);
static void vTaskRefresh(void *pvParameters);
static void vTaskADC(void *pvParameters);
static void ADC_Configuration(void);
static void SPI_Configuration(void);
static void GPIO_Configuration(void);
static void OLED_Configuration(void);
static void effect_switch_callback(pint_pin_int_t pintr, uint32_t pmatch_status);
static void effect_mode_callback(pint_pin_int_t pintr, uint32_t pmatch_status);

/******************************************************************************
 * Functions
 ******************************************************************************/

/*------------------------------------------------------------------------------
 Cube OS/IO APIs implementations
 -----------------------------------------------------------------------------*/

/*
 * Board devices initialisations
 */
void ledQB_board_init(void) {
	GPIO_Configuration();
	SPI_Configuration();
	ADC_Configuration();
	OLED_Configuration();
}

/*
 * OLED Display initialisation
 */
static void OLED_Configuration(void) {
	i2c_master_config_t masterConfig;

	I2C_MasterGetDefaultConfig(&masterConfig);
	masterConfig.baudRate_Bps = 400000U;

	I2C_MasterInit(I2C_MASTER, &masterConfig, CLOCK_GetFreq(kCLOCK_MainClk));

	Display_Init();
	Display_Clear();
	Display_Update();
}

/*
 * GPIO Display initialisation
 */
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

/*
 * SPI Display initialisation
 */
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
	userConfig.delayConfig.postDelay = 0x00;
	userConfig.delayConfig.preDelay = 0x00;
	userConfig.delayConfig.frameDelay = 0x00;
	userConfig.delayConfig.transferDelay = 0x00;

	srcFreq = CLOCK_GetFreq(kCLOCK_MainClk);
	SPI_MasterInit(SPI0, &userConfig, srcFreq);
}

/*
 * ADC Display initialisation
 */
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

/*
 * OLED Display update
 */
static void displayUpdate(char *text) {
	uint8_t column = 6 * strlen(STR_EFFECT);
	Display_ScrollStop();

	Display_Clear();

	Display_Line(0, 2, DISPLAY_WIDTH - 2, 2);
	Display_SetFont(ssd1306xled_font8x16);
	Display_Text(0, 5, LEDQB_FULL_NAME);
	Display_Line(0, 26, DISPLAY_WIDTH - 2, 26);

	Display_SetFont(&ssd1306xled_font6x8[0]);
	Display_Text(0, 35, STR_EFFECT);
	Display_Text(column, 35, text);
	Display_Text(0, 45, STR_MODE);
	Display_Text(0, 55, STR_STATS);

	sprintf(score, "P=%d G=%d", max_population, max_generations);
	Display_Text(column, 55, score);

	switch (mode) {
	case MODE_ALL: {
		Display_Text(column, 45, STR_MODE_ALL_EFFECTS);
	}
		break;
	case MODE_FIXED: {
		Display_Text(column, 45, STR_MODE_FIXED);
	}
		break;
	case MODE_VOLUME: {
		Display_Text(column, 45, STR_MODE_VOLUME);
	}
		break;
	case MODE_LIFE: {
		Display_Text(column, 45, STR_MODE_LIFE);
	}
		break;
	}

	Display_Update();
	Display_ScrollLeft(1, 2);
}

/*
 * Low-level Display I2C IO data streams
 */
void SSD1306_Data(uint8_t *data, size_t size) {
	i2c_master_transfer_t xfer = { 0 };

	xfer.data = data;
	xfer.dataSize = size;
	xfer.flags = kI2C_TransferDefaultFlag;
	xfer.slaveAddress = SSD1306_7BITS_ADDRESS;
	xfer.direction = kI2C_Write;
	xfer.subaddress = SSD1306_DATA; /* Data */
	xfer.subaddressSize = 1;

	I2C_MasterTransferBlocking(I2C_MASTER, &xfer);
}

/*
 * Low-level Display I2C IO command
 */
void SSD1306_Command(uint8_t command) {
	i2c_master_transfer_t xfer = { 0 };

	xfer.data = (uint8_t *) &command;
	xfer.dataSize = sizeof(command);
	xfer.flags = kI2C_TransferDefaultFlag;
	xfer.slaveAddress = SSD1306_7BITS_ADDRESS;
	xfer.direction = kI2C_Write;
	xfer.subaddress = SSD1306_COMMAND; /* Command */
	xfer.subaddressSize = 1;

	I2C_MasterTransferBlocking(I2C_MASTER, &xfer);
}

/*------------------------------------------------------------------------------
 Cube IO specific functions implementation (i.e. weak symbols redefinition)
 -----------------------------------------------------------------------------*/
void ledQB_board_plane_select(uint8_t plane) {
	GPIO_PortClear(GPIO, BOARD_PORT, 1u << PLANE_DEC_EN);
	uint32_t mask = ledQB_board_plane_mask(plane,
			GPIO_PortRead(GPIO, BOARD_PORT),
			PLANE_DEC_BIT0, PLANE_DEC_BIT1, PLANE_DEC_BIT2, PLANE_DEC_EN);

	GPIO_PortMaskedWrite(GPIO, BOARD_PORT, mask);
}

void ledQB_board_plane_unselect(uint8_t plane) {
	GPIO_PortSet(GPIO, BOARD_PORT, 1u << PLANE_DEC_EN);
}

void ledQB_board_plane_send(uint8_t *data, uint8_t size) {
	spi_transfer_t xfer = { 0 };

	xfer.txData = data;
	xfer.dataSize = size;
	xfer.configFlags = kSPI_EndOfFrame | kSPI_EndOfTransfer
			| kSPI_ReceiveIgnore;
	SPI_MasterTransferBlocking(SPI0, &xfer);
}

void ledQB_osal_lock(void) {
	xSemaphoreTake(mutex_v, portMAX_DELAY);
}

void ledQB_osal_unlock(void) {
	xSemaphoreGive(mutex_v);
}

void ledQB_osal_sleep(uint32_t sleep_ms) {
	vTaskDelay(sleep_ms / portTICK_PERIOD_MS);
}

uint32_t ledQB_osal_time_now(void) {
	return xTaskGetTickCount() * portTICK_PERIOD_MS;
}

void ledQB_osal_init(void) {

	mutex_v = xSemaphoreCreateMutex();

	xTaskCreate(vTaskEffects, "vTaskEffects", configMINIMAL_STACK_SIZE, NULL,
			(tskIDLE_PRIORITY + 1UL), (TaskHandle_t *) NULL);

	xTaskCreate(vTaskRefresh, "vTaskRefresh", 64, NULL,
			(tskIDLE_PRIORITY + 1UL), (TaskHandle_t *) NULL);

	xTaskCreate(vTaskADC, "vTaskADC", 32, NULL, (tskIDLE_PRIORITY + 1UL),
			(TaskHandle_t *) NULL);
}

/*------------------------------------------------------------------------------
 Handlers
 -----------------------------------------------------------------------------*/
uint16_t life_effect_callback(void) {
	char *effect = ledQB_get_currentEffect();
	displayUpdate(effect);

	return 0;
}

uint16_t adc_effect_callback(void) {
	uint16_t signalMax = 0;
	uint16_t signalMin = UINT16_MAX;
	uint16_t value = 0;
	uint16_t i = 0;

	for (i = 0; i < ADC_BUFFER_DEPTH; i++) {
		value = adc_samples[i];
		if (value > signalMax) {
			signalMax = value;
		} else if (value < signalMin) {
			signalMin = value;
		}
	}

	return map((signalMax - signalMin), 0, max_sensivity, 0, LEDQB_SIZE);
}

static void effect_switch_callback(pint_pin_int_t pintr, uint32_t pmatch_status) {

	switch (mode) {
	case MODE_ALL: {
		ledQB_quit_effect();
	}
		break;
	case MODE_FIXED: {
		/* Do nothing */
	}
		break;
	case MODE_VOLUME: {
		max_sensivity = max_sensivity+ADC_DEFAULT_INCREMENT;
		if ( max_sensivity < ADC_DEFAULT_MIN)
			max_sensivity = ADC_DEFAULT_MAX;
	}
		break;
	case MODE_LIFE: {
		/* Do nothing */
	}
		break;
	}

}

static void effect_mode_callback(pint_pin_int_t pintr, uint32_t pmatch_status) {
	mode = (mode + 1) % MODES;

	/* Turn off LEDs */
	GPIO_PortSet(GPIO, BOARD_PORT,
			1u << BOARD_LED_BLUE_PIN | 1u << BOARD_LED_GREEN_PIN);

	switch (mode) {
	case MODE_ALL: {
		ledQB_set_runMode("");
		GPIO_PortSet(GPIO, BOARD_PORT, 1u << BOARD_LED_BLUE_PIN);
	}
		break;
	case MODE_FIXED: {
		char *effect = ledQB_get_currentEffect();
		ledQB_set_runMode(effect);
		GPIO_PortClear(GPIO, BOARD_PORT, 1u << BOARD_LED_BLUE_PIN);
	}
		break;
	case MODE_VOLUME: {
		ledQB_set_runMode(STR_MODE_VOLUME);
		GPIO_PortSet(GPIO, BOARD_PORT, 1u << BOARD_LED_GREEN_PIN);
	}
		break;
	case MODE_LIFE: {
		ledQB_set_runMode("Life");
		GPIO_PortClear(GPIO, BOARD_PORT, 1u << BOARD_LED_GREEN_PIN);
	}
		break;
	}

	char *effect = ledQB_get_currentEffect();
	displayUpdate(effect);
}

void ADC0_SEQA_IRQHandler(void) {
	if (kADC_ConvSeqAInterruptFlag
			== (kADC_ConvSeqAInterruptFlag & ADC_GetStatusFlags(ADC0))) {
		ADC_EnableConvSeqABurstMode(ADC0, false);

		ADC_GetChannelConversionResult(ADC0, ADC_SAMPLE_CHANNEL,
				gAdcResultInfoPtr);
		ADC_ClearStatusFlags(ADC0, kADC_ConvSeqAInterruptFlag);
        gAdcConvSeqAIntFlag = true;

	}
}

/*------------------------------------------------------------------------------
 Application OS threads
 -----------------------------------------------------------------------------*/

/*
 * Cube effects sequence thread
 */
static void vTaskEffects(void *pvParameters) {
	uint8_t i = 0;
	char *oldEffect = NULL;

	while (true) {
		if (mode == MODE_ALL) {
			char *effect = ledQB_get_Effect(i);
			if (oldEffect != effect)
				displayUpdate(effect);
		}

		if (ledQB_effects(i))
			i = 0;
		else
			i++;

		vTaskDelay(1);
	}
}

/*
 * Cube refresh thread
 */
static void vTaskRefresh(void *pvParameters) {
	while (true) {
		ledQB_refresh();
		vTaskDelay(1);
	}
}

/*
 * ADC sampling thread
 */
static void vTaskADC(void *pvParameters) {
	uint8_t sample = 0;

	while (true) {
		ADC_EnableConvSeqABurstMode(ADC0, true);
		while (!gAdcConvSeqAIntFlag) {
			vTaskDelay(1);
		}
		adc_samples[sample++ % ADC_BUFFER_DEPTH] = ((gAdcResultInfoStruct.result
				& ADC_DAT_RESULT_MASK) >> ADC_DAT_RESULT_SHIFT);
	}
}

/*------------------------------------------------------------------------------
 Application entry point
 -----------------------------------------------------------------------------*/
int main(void) {
	BOARD_InitBootPins();
	BOARD_InitBootClocks();

	ledQB_init();

	GPIO_PortSet(GPIO, BOARD_PORT, 1u << BOARD_LED_GREEN_PIN);
	if (strlen(ledQB_get_runMode()) == 0)
		GPIO_PortSet(GPIO, BOARD_PORT, 1u << BOARD_LED_BLUE_PIN);
	else
		GPIO_PortClear(GPIO, BOARD_PORT, 1u << BOARD_LED_BLUE_PIN);

	vTaskStartScheduler();

	return 0;
}
