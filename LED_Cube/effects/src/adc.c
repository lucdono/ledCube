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
#include <math.h>

#include "cube.h"
#include "cube_math.h"
#include "cube_interface.h"
#include "adc.h"

/******************************************************************************
 * Functions
 ******************************************************************************/
void f_adc(uint16_t frame) {
	uint8_t y = 0;
	uint8_t z = 0;
	uint8_t height = 0;

	(void) frame;

	height = adc_effect_callback();

	ledQB_shiftXLayer(true);
	ledQB_clrXLayer(LEDQB_SIZE - 1);

	for (y = 0; y < LEDQB_SIZE; y++) {
		for (z = 0; z < height; z++) {
			point_t point = { LEDQB_SIZE - 1, y, z, 1 };
			ledQB_point(point);
		}
	}
}

/*------------------------------------------------------------------------------
 Weak symbols for effect callbacks
 ------------------------------------------------------------------------------*/
__attribute__ ((weak)) uint16_t adc_effect_callback(void) { return 0; }
