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
#include <stdlib.h>

#include "cube.h"
#include "cube_math.h"
#include "spin.h"

/******************************************************************************
 * Functions
 ******************************************************************************/
void f_spin(uint16_t frame) {
	uint8_t x = 0;
	uint8_t y = 0;

	uint16_t angle = frame % A_360;

	int16_t sinTheta = sin_table[(angle + A_180) % A_360];
	int16_t cosTheta = sin_table[(angle + A_180 + A_90) % A_360];

	for (x = 0; x < LEDQB_SIZE; x++) {
		for (y = 0; y < LEDQB_SIZE; y++) {
			int32_t az = sinTheta * (y - LEDQB_SIZE / 2);
			int32_t bz = cosTheta * (x - LEDQB_SIZE / 2);

			int8_t Z = map(-az + bz,
					-LEDQB_SIZE/2*CUBEMATH_SIN, LEDQB_SIZE/2*CUBEMATH_SIN, 0,
					LEDQB_SIZE);

			point_t point = { x, y, Z, 1 };
			ledQB_point(point);
		}
	}

}
