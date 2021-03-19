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
#include "rain.h"

/******************************************************************************
 * Functions
 ******************************************************************************/
void f_rain(uint16_t frame) {
	(void) frame;

	uint8_t z = 0, i = 0;
	uint8_t drops = rand() % RAIN_DROPS;
	uint8_t layer[LEDQB_SIZE];

	memset(layer, 0, LEDQB_SIZE * sizeof(uint8_t));
	for (z = 0; z < LEDQB_SIZE - 1; z++) {
		memcpy(layer, ledQB_getZLayer(z + 1), LEDQB_SIZE * sizeof(uint8_t));
		ledQB_clrLayer(z);
		ledQB_setZLayer(layer, z);
	}

	ledQB_clrLayer(LEDQB_SIZE - 1);

	for (i = 0; i < drops; i++) {
		uint8_t x = rand() % LEDQB_SIZE;
		uint8_t y = rand() % LEDQB_SIZE;

		point_t point = { x, y, LEDQB_SIZE - 1, 1 };
		ledQB_point(point);
	}
}

