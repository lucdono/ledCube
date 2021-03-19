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

#ifndef _CUBEMATH_H_
#define _CUBEMATH_H_

/******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdint.h>

/******************************************************************************
 * Defines
 ******************************************************************************/
#define A_180	180
#define A_0	    0
#define A_90	90
#define A_360	360

/*
 * Number of lookup table entries
 */
#define CUBEMATH_SIN	10000

/*
 * Scale a value in range (a,b) to a value in range (x,y)
 */
#define map(value, a, b, x, y) (((float)(value - a)) / ((float)(b - a)) * ((float)(y - x)) + (float)x)

/******************************************************************************
 * External Variables
 ******************************************************************************/

/*
 * Lookup table
 */
extern int16_t sin_table[A_360];

/******************************************************************************
 * Prototypes
 ******************************************************************************/
uint16_t ledQB_sqrt(uint16_t a_nInput);

#endif /* _CUBEMATH_H_ */
