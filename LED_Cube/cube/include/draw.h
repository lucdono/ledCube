#ifndef _DRAW_H_
#define _DRAW_H_

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "cube.h"

/******************************************************************************
 * Types
 ******************************************************************************/
typedef uint8_t color_t;

typedef struct {
	uint8_t x;
	uint8_t y;
	uint8_t z;
	color_t color;
} point_t;

typedef struct {
	point_t start;
	point_t end;
	color_t color;
} line_t;

typedef enum plane {
	XY = 0, YZ = 1, ZX = 2
} plane_t;

/******************************************************************************
 * Defines
 ******************************************************************************/
#define ledQB_inRange(p)	(p.x < LEDQB_SIZE && \
						 	 p.y < LEDQB_SIZE && \
							 p.z < LEDQB_SIZE )

#define flip(f, x)   		( f ? (LEDQB_SIZE-1)-x : x )

/******************************************************************************
 * Prototypes
 ******************************************************************************/
void ledQB_clear(void);
void ledQB_point(point_t p);
void ledQB_line(line_t r);
void ledQB_shiftXLayer(uint8_t dir);
void ledQB_clrXLayer(uint8_t index);
void ledQB_setZLayer(uint8_t *layer, uint8_t index);
void ledQB_clrLayer(uint8_t index);
void ledQB_orLayer(uint8_t *layer, uint8_t index);
void ledQB_plane(uint8_t plane, uint8_t axis);
void ledQB_char(plane_t plane, uint8_t axis, char c, uint8_t f, uint8_t offset);
uint8_t ledQB_cleared(void);
uint8_t *ledQB_getZLayer(uint8_t index);
color_t ledQB_getPoint(uint8_t x, uint8_t y, uint8_t z);

#endif /* DRAW_H_ */
