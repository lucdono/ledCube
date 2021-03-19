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

#ifndef WRITE_H_
#define WRITE_H_

/******************************************************************************
 * Defines
 ******************************************************************************/
#define MAX_STRING			50

#define MODE_YZ				0
#define MODE_SURROUND		1

/******************************************************************************
 * Prototypes
 ******************************************************************************/
void f_write_init(void);
void f_write(uint16_t frame);

#endif /* WRITE_H_ */
