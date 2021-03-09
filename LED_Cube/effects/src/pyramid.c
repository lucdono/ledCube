/******************************************************************************
 * Includes
 ******************************************************************************/
#include "cube.h"
#include "pyramid.h"

/******************************************************************************
 * Internal Variables
 ******************************************************************************/
static uint8_t size = 0;
static uint8_t fill = 1;

/******************************************************************************
 * Functions
 ******************************************************************************/
void f_pyramid_init(void) {
	size = 0;
	fill = 1;
}

void f_pyramid(uint16_t frame) {
	(void) frame;

	uint8_t x = 0;
	uint8_t y = 0;
	uint8_t z = 0;

	for (x = 0; x <= size; x++) {
		for (y = 0; y <= size; y++) {
			z = size - (x + y);
			point_t point = { x, y, z, fill };
			ledQB_point(point);
		}
	}

	if (size == 3 * (LEDQB_SIZE - 1)) {
		fill = !fill;
		size = 0;
	} else {
		size++;
	}

}
