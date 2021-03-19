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

#ifndef _DRAW_H_
#define _DRAW_H_

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "cube.h"

/******************************************************************************
 * Types
 ******************************************************************************/

/*
 * Pixel colour information
 */
typedef uint8_t color_t;

/*
 * 3D Point structure
 */
typedef struct {
	uint8_t x;
	uint8_t y;
	uint8_t z;
	color_t color;
} point_t;

/*
 * 3D Line structure
 */
typedef struct {
	point_t start;
	point_t end;
	color_t color;
} line_t;

/*
 * 3D planes enumerations
 */
typedef enum plane {
	XY = 0, YZ = 1, ZX = 2
} plane_t;

/******************************************************************************
 * Defines
 ******************************************************************************/

/*
 * Check if a point belongs to the Cube
 */
#define ledQB_inRange(p)	(p.x < LEDQB_SIZE && \
						 	 p.y < LEDQB_SIZE && \
							 p.z < LEDQB_SIZE )

/*
 * Flip an axis value x if f is true
 */
#define flip(f, x)   					( f ? (LEDQB_SIZE-1)-x : x )

/*
 * Returns the Z layer selected by index
 */
#define ledQB_getZLayer(index)  		(frame_buffer[index])

/*
 * Set the whole Z layer selected by index
 */
#define ledQB_setZLayer(layer, index) 	(memcpy(frame_buffer[index], layer, LEDQB_SIZE * sizeof(uint8_t)))

/*
 * Clear Cube memory buffer
 */
#define ledQB_clear()					(memset(frame_buffer, 0, LEDQB_SIZE * LEDQB_SIZE * sizeof(uint8_t)))

/*
 * Set Cube memory buffer
 */
#define ledQB_full()					(memset(frame_buffer, 0xFF, LEDQB_SIZE * LEDQB_SIZE * sizeof(uint8_t)))

/*
 * Clear the Z layer selected by index
 */
#define ledQB_clrLayer(index)			(memset(frame_buffer[index], 0, LEDQB_SIZE * sizeof(uint8_t)))

/******************************************************************************
 * Prototypes
 ******************************************************************************/

/*
 * Draw a point
 */
void ledQB_point(point_t p);

/*
 * Draw a line
 */
void ledQB_line(line_t r);

/*
 * Shift all X layers of one position forward (dir=0) or backward (dir=1)
 */
void ledQB_shiftXLayer(uint8_t dir);

/*
 * Clear an X layer selected by index
 */
void ledQB_clrXLayer(uint8_t index);

/*
 * Point-to-point or of a layer pointed by 'layer' with another layer selected by index
 */
void ledQB_orLayer(uint8_t *layer, uint8_t index);

/*
 * Draw a Plane at axis, according to its 'plane' orientation
 */
void ledQB_plane(uint8_t plane, uint8_t axis);

/*
 * Draw a char on a specific 'plane' at 'axis' + 'offset' position
 */
void ledQB_char(plane_t plane, uint8_t axis, char c, uint8_t f, uint8_t offset);

/*
 * Get the point information at coordinate x,y,z
 */
color_t ledQB_getPoint(uint8_t x, uint8_t y, uint8_t z);

#endif /* DRAW_H_ */
