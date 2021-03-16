/******************************************************************************
 * Includes
 ******************************************************************************/
#include <time.h>
#include <stdlib.h>

#include "cube.h"
#include "cube_interface.h"
#include "bitfields.h"
#include "effects.h"

/******************************************************************************
 * Internal Variables
 ******************************************************************************/
static uint8_t plane = 0;

/******************************************************************************
 * Variables
 ******************************************************************************/
uint8_t frame_buffer[LEDQB_SIZE][LEDQB_SIZE];

/******************************************************************************
 * Defines
 ******************************************************************************/
#define	PLANE_MASK(b0,b1,b2,en)  ( ~(BIT(b0)|BIT(b1)|BIT(b2)|BIT(en)) )

#define	PLANE_VALUE(y,b0,b1,b2)  ( BIT_SHIFT((y & 0x04)>>2, b2) | \
                                   BIT_SHIFT((y & 0x02)>>1, b1) | \
                                   BIT_SHIFT(y & 0x01, b0))

/******************************************************************************
 * API
 ******************************************************************************/
void ledQB_init(void) {
	srand(time(NULL));

	ledQB_set_runMode(DEFAULT_RUN_MODE);
	ledQB_board_init();
	ledQB_osal_init();
}

void ledQB_refresh(void) {
	ledQB_osal_lock();
	/* Turn FET off to avoid ghost effect */
	ledQB_board_plane_unselect(plane);
	ledQB_board_plane_send(frame_buffer[plane], LEDQB_SIZE);
	ledQB_board_plane_select(plane);
	plane = (plane + 1) % LEDQB_SIZE;
	ledQB_osal_unlock();
}

uint32_t ledQB_board_plane_mask(uint8_t plane, uint32_t portStatus, uint8_t pin0,
		uint8_t pin1, uint8_t pin2, uint8_t pinEnable) {
	uint32_t mask = (portStatus & PLANE_MASK(pin0, pin1, pin2, pinEnable))
			| PLANE_VALUE((plane + 1), pin0, pin1, pin2);
	return mask;
}

__attribute__ ((weak)) void ledQB_board_init(void) {}
__attribute__ ((weak)) void ledQB_board_plane_select(uint8_t plane) {}
__attribute__ ((weak)) void ledQB_board_plane_unselect(uint8_t plane) {}
__attribute__ ((weak)) void ledQB_board_plane_send(uint8_t *data, uint8_t size) {}
__attribute__ ((weak)) void ledQB_osal_sleep(uint32_t sleep_ms) {}
__attribute__ ((weak)) uint32_t ledQB_osal_time_now(void) { return 0; }
__attribute__ ((weak)) void ledQB_osal_lock(void) {}
__attribute__ ((weak)) void ledQB_osal_unlock(void) {}
__attribute__ ((weak)) void ledQB_osal_init(void) {}


