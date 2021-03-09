/******************************************************************************
 * Includes
 ******************************************************************************/
#include "clock_config.h"
#include "board.h"
#include "pin_mux.h"

#include "fsl_spi.h"
#include "fsl_gpio.h"
#include "fsl_pint.h"
#include "fsl_syscon.h"

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

/******************************************************************************
 * Internal Variables
 ******************************************************************************/
static SemaphoreHandle_t mutex_v;

/******************************************************************************
 * Prototypes
 ******************************************************************************/
static void vTaskEffects(void *pvParameters);
static void vTaskRefresh(void *pvParameters);

/******************************************************************************
 * Functions
 ******************************************************************************/

/*------------------------------------------------------------------------------
 Cube OS/IO APIs implementations
 -----------------------------------------------------------------------------*/

void ledQB_board_init(void) {
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

void ledQB_board_plane_select(uint8_t plane) {
	uint32_t mask = ledQB_board_plane_mask(plane, GPIO_PortRead(GPIO, BOARD_PORT),
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
}

/*------------------------------------------------------------------------------
 Switches Callback
 -----------------------------------------------------------------------------*/

void effect_switch_callback(pint_pin_int_t pintr, uint32_t pmatch_status) {
	ledQB_quit_effect();
}

void effect_mode_callback(pint_pin_int_t pintr, uint32_t pmatch_status) {
	char *effect = ledQB_get_currentEffect();
	char *currentMode = ledQB_get_runMode();
	if (effect != NULL && strlen(currentMode) == 0) {
		ledQB_set_runMode(effect);
		GPIO_PortClear(GPIO, BOARD_PORT, 1u << BOARD_LED_BLUE_PIN);
	} else {
		ledQB_set_runMode("");
		GPIO_PortSet(GPIO, BOARD_PORT, 1u << BOARD_LED_BLUE_PIN);
	}
}

/*------------------------------------------------------------------------------
 Application OS threads
 -----------------------------------------------------------------------------*/

static void vTaskEffects(void *pvParameters) {
	const TickType_t xDelay = 1 / portTICK_PERIOD_MS;
	uint8_t i = 0;

	while (true) {
		if (ledQB_effects(i))
			i = 0;
		else
			i++;

		GPIO_PortToggle(GPIO, BOARD_PORT, 1u << BOARD_LED_GREEN_PIN);
		vTaskDelay(xDelay);
	}
}

static void vTaskRefresh(void *pvParameters) {
	const TickType_t xDelay = 3 / portTICK_PERIOD_MS;
	while (true) {
		ledQB_refresh();
		vTaskDelay(xDelay);
	}
}

/*------------------------------------------------------------------------------
 Application entry point
 -----------------------------------------------------------------------------*/
int main(void) {
	BOARD_InitBootPins();
	BOARD_InitBootClocks();

	SYSCON_AttachSignal(SYSCON, kPINT_PinInt0, kSYSCON_GpioPort0Pin1ToPintsel);
	PINT_PinInterruptConfig(PINT, kPINT_PinInt0, kPINT_PinIntEnableRiseEdge,
			effect_switch_callback);
	PINT_EnableCallbackByIndex(PINT, kPINT_PinInt0);

	SYSCON_AttachSignal(SYSCON, kPINT_PinInt1, kSYSCON_GpioPort0Pin6ToPintsel);
	PINT_PinInterruptConfig(PINT, kPINT_PinInt1, kPINT_PinIntEnableRiseEdge,
			effect_mode_callback);
	PINT_EnableCallbackByIndex(PINT, kPINT_PinInt1);

	ledQB_init();
	if (strlen(ledQB_get_runMode()) == 0)
		GPIO_PortSet(GPIO, BOARD_PORT, 1u << BOARD_LED_BLUE_PIN);
	else
		GPIO_PortClear(GPIO, BOARD_PORT, 1u << BOARD_LED_BLUE_PIN);

	vTaskStartScheduler();

	return 0;
}
