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

