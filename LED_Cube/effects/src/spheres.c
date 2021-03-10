/******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdlib.h>

#include "cube.h"
#include "cube_math.h"
#include "spheres.h"

/******************************************************************************
 * Internal Variables
 ******************************************************************************/
static int8_t dir_s = 1;
static uint8_t radius = 0;
static uint8_t cx = (LEDQB_SIZE - 1) / 2;
static uint8_t cy = (LEDQB_SIZE - 1) / 2;
static uint8_t cz = (LEDQB_SIZE - 1) / 2;
static uint32_t start = 0;

/******************************************************************************
 * Functions
 ******************************************************************************/
void f_spheres_init(void) {
	dir_s = 1;
	radius = 0;
	cx = (LEDQB_SIZE - 1) / 2;
	cy = (LEDQB_SIZE - 1) / 2;
	cz = (LEDQB_SIZE - 1) / 2;
	start = 0;
}

void f_spheres(uint16_t frame) {
	(void) frame;

	uint8_t x = 0;
	uint8_t y = 0;
	uint8_t z = 0;

	radius = radius + dir_s;

	for(x = 0; x < LEDQB_SIZE; x++){
		for(y = 0; y < LEDQB_SIZE; y++){
			for(z = 0; z < LEDQB_SIZE; z++){
				uint16_t value = (x-cx)*(x-cx)+(y-cy)*(y-cy)+(z-cz)*(z-cz);
				point_t point = { x, y, z, value <= (radius*radius) ? 1 : 0 };
				ledQB_point(point);
			}
		}
	}

	if (radius== LEDQB_SIZE / 2)
		dir_s = -1;
	else if (radius == 0)
		dir_s = 1;

	if (ledQB_osal_time_now() - start > 2000) {
		start = ledQB_osal_time_now();
		cx = rand() % (LEDQB_SIZE - 1);
		cy = rand() % (LEDQB_SIZE - 1);
		cz = rand() % (LEDQB_SIZE - 1);
	}
}

