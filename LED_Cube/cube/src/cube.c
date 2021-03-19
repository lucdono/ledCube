/*
 * 3D LED CUBE
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
#include <time.h>
#include <stdlib.h>

#include "cube.h"
#include "cube_interface.h"
#include "bitfields.h"
#include "effects.h"

/******************************************************************************
 * Internal Variables
 ******************************************************************************/

/*
 * Selected plane related to the cube refreshing sequence
 */
static uint8_t plane = 0;

/******************************************************************************
 * Variables
 ******************************************************************************/
uint8_t frame_buffer[LEDQB_SIZE][LEDQB_SIZE];

/******************************************************************************
 * Defines
 ******************************************************************************/

/*
 * Create a mask for the Z layer selection
 */
#define	PLANE_MASK(b0,b1,b2,en)  ( ~(BIT(b0)|BIT(b1)|BIT(b2)|BIT(en)) )

/*
 * Determine the value of the Z layer selection to pass to the 8-bit decoder
 */
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

	/*
	 * Refresh algorithm, to light a led at a specific Z layer needs to
	 * continuously scan all planes vertically: if the refresh rate is high
	 * enough, the human eye will just see a stable image of various leds turned
	 * on at different Z positions.
	 */
	plane = (plane + 1) % LEDQB_SIZE;

	ledQB_osal_unlock();
}

uint32_t ledQB_board_plane_mask(uint8_t plane, uint32_t portStatus,
		uint8_t pin0, uint8_t pin1, uint8_t pin2, uint8_t pinEnable) {
	uint32_t mask = (portStatus & PLANE_MASK(pin0, pin1, pin2, pinEnable))
			| PLANE_VALUE((plane + 1), pin0, pin1, pin2);
	return mask;
}

/*------------------------------------------------------------------------------
 Weak symbols for functions to be implemented for a specific board and OS
 ------------------------------------------------------------------------------*/
__attribute__ ((weak)) void ledQB_board_init(void) {}
__attribute__ ((weak)) void ledQB_board_plane_select(uint8_t plane) {}
__attribute__ ((weak)) void ledQB_board_plane_unselect(uint8_t plane) {}
__attribute__ ((weak)) void ledQB_board_plane_send(uint8_t *data, uint8_t size) {}
__attribute__ ((weak)) void ledQB_osal_sleep(uint32_t sleep_ms) {}
__attribute__ ((weak)) uint32_t ledQB_osal_time_now(void) {	return 0; }
__attribute__ ((weak)) void ledQB_osal_lock(void) {}
__attribute__ ((weak)) void ledQB_osal_unlock(void) {}
__attribute__ ((weak)) void ledQB_osal_init(void) {}

