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

#ifndef LIFE_H_
#define LIFE_H_

/******************************************************************************
 * Defines
 ******************************************************************************/
#define SEEDS		20

#define ALIVE_N 	3
#define ALONE_N 	2
#define OVER_N 		4

/******************************************************************************
 * Prototypes
 ******************************************************************************/
void f_life_init(void );
void f_life(uint16_t frame);

uint16_t life_effect_callback(void);

#endif /* LIFE_H_ */
