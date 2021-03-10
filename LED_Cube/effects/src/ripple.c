/******************************************************************************
 * Includes
 ******************************************************************************/
#include "cube.h"
#include "cube_math.h"
#include "ripple.h"

/******************************************************************************
 * Internal Variables
 ******************************************************************************/
static int16_t mapX;
static int16_t mapY;
static uint16_t dist;

/******************************************************************************
 * Functions
 ******************************************************************************/
void f_ripple(uint16_t frame) {
	uint8_t x = 0;
	uint8_t y = 0;
	uint8_t z = 0;

	for (y = 0; y < LEDQB_SIZE; y++) {
		mapY = map(y, 0, LEDQB_SIZE - 1, -A_180, A_180);
		for (x = 0; x < LEDQB_SIZE; x++) {
			mapX = map(x, 0, LEDQB_SIZE - 1, -A_180, A_180);
			dist = ledQB_sqrt(mapX * mapX + mapY * mapY);

			z = map(sin_table[(frame + dist)%A_360], -CUBEMATH_SIN,
					CUBEMATH_SIN, 0, LEDQB_SIZE);
			point_t point = { x, y, z, 1 };
			ledQB_point(point);
		}
	}
}

