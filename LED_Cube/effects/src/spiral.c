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
#include "cube.h"
#include "cube_math.h"
#include "spiral.h"

/******************************************************************************
 * Functions
 ******************************************************************************/
void f_spiral(uint16_t frame) {
	uint8_t x = 0;
	uint8_t y = 0;
	uint8_t z = 0;

	for (z = 0; z < LEDQB_SIZE; z++) {
		int16_t theta = map(z, 0, LEDQB_SIZE - 1, (-A_180 + frame),
				(A_180 + frame));
		int16_t X = map(sin_table[(theta+A_180) % A_360], -CUBEMATH_SIN,
				CUBEMATH_SIN, 0, LEDQB_SIZE);
		int16_t Y = map(sin_table[(theta + A_90 + A_180) % A_360], -CUBEMATH_SIN,
				CUBEMATH_SIN, 0, LEDQB_SIZE);

		for (x = 0; x < LEDQB_SIZE; x++) {
			for (y = 0; y < LEDQB_SIZE; y++) {
				if (((x == X) || (x == X - 1) || (x == X - 2)) && (y == Y)) {
					point_t point = { x, y, z, 1 };
					ledQB_point(point);
				}
			}
		}
	}
}
