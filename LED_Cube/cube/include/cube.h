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

#ifndef _LEDQB_H_
#define _LEDQB_H_

/******************************************************************************
 * Includes
 ******************************************************************************/
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "draw.h"
#include "cube_interface.h"

/******************************************************************************
 * Defines
 ******************************************************************************/
#define _LEDQB_

/******************************************************************************
 * External Variables
 ******************************************************************************/
/*
 * Data structure for cube that allows double buffering but not the intensity
 * regulation.
 * X dim information is hidden into uint8_t
 */
extern uint8_t frame_buffer[LEDQB_SIZE][LEDQB_SIZE];

#endif /* _LEDQB_H_ */
