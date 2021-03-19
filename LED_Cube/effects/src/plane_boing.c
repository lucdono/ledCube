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
#include "plane_boing.h"

/******************************************************************************
 * Functions
 ******************************************************************************/
void f_plane_boing(uint16_t frame) {
	uint8_t axis = 0;

	frame = frame % 48;
	if (frame < 16) {
		if (frame < LEDQB_SIZE)
			axis = frame;
		else
			axis = 16 - frame - 1;

		ledQB_plane(ZX, axis);
	} else if (frame >= 16 && frame < 32) {
		if (frame - 16 < LEDQB_SIZE)
			axis = frame - 16;
		else
			axis = 32 - frame - 1;
		ledQB_plane(XY, axis);
	} else {
		if (frame - 32 < LEDQB_SIZE)
			axis = frame - 32;
		else
			axis = 48 - frame - 1;

		ledQB_plane(YZ, axis);
	}
}

