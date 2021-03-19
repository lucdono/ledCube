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
#include "pyramid.h"

/******************************************************************************
 * Internal Variables
 ******************************************************************************/
static uint8_t size = 0;
static uint8_t fill = 1;

/******************************************************************************
 * Functions
 ******************************************************************************/
void f_pyramid_init(void) {
	size = 0;
	fill = 1;
}

void f_pyramid(uint16_t frame) {
	(void) frame;

	uint8_t x = 0;
	uint8_t y = 0;

	for (x = 0; x <= size; x++) {
		for (y = 0; y <= size; y++) {
			point_t point = { x, y, size - (x + y), fill };
			ledQB_point(point);
		}
	}

	if (size == 3 * (LEDQB_SIZE - 1)) {
		fill = !fill;
		size = 0;
	} else {
		size++;
	}

}
