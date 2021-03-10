/******************************************************************************
 * Includes
 ******************************************************************************/
#include "cube.h"
#include "cubes.h"

/******************************************************************************
 * Defines
 ******************************************************************************/
#define EDGES		12

/******************************************************************************
 * Internal Variables
 ******************************************************************************/
static int8_t dir_c = 1;
static uint8_t size = 0;
static uint8_t fill = 0;
static line_t lines[EDGES] = {
		{{0, 0, 0}, {1, 0, 0}, 1},
		{{1, 0, 0}, {1, 1, 0}, 1},
		{{1, 1, 0}, {0, 1, 0}, 1},
		{{0, 1, 0}, {0, 0, 0}, 1},
		{{0, 0, 1}, {1, 0, 1}, 1},
		{{1, 0, 1}, {1, 1, 1}, 1},
		{{1, 1, 1}, {0, 1, 1}, 1},
		{{0, 1, 1}, {0, 0, 1}, 1},
		{{0, 0, 0}, {0, 0, 1}, 1},
		{{1, 0, 0}, {1, 0, 1}, 1},
		{{1, 1, 0}, {1, 1, 1}, 1},
		{{0, 1, 0}, {0, 1, 1}, 1},
};

/******************************************************************************
 * Functions
 ******************************************************************************/
void f_cubes_init(void) {
	dir_c = 1;
	size = 0;
	fill = 0;
}

void f_cubes(uint16_t frame) {
	(void) frame;
	line_t line;
	uint8_t i = 0;
	uint8_t x = 0;
	uint8_t y = 0;
	uint8_t z = 0;

	size = size + dir_c;

	if (!fill) {
		for (i = 0; i < EDGES; i++) {
			line.start.x = size*lines[i].start.x;
			line.start.y = size*lines[i].start.y;
			line.start.z = size*lines[i].start.z;
			line.end.x = size*lines[i].end.x;
			line.end.y = size*lines[i].end.y;
			line.end.z = size*lines[i].end.z;
			line.color = 1;

			ledQB_line(line);
		}
	} else {
		for (x = 0; x <= size; x++) {
			for (y = 0; y <= size; y++) {
				for (z = 0; z <= size; z++) {
					point_t point = { x, y, z, 1 };
					ledQB_point(point);
				}
			}
		}
	}

	if (size == LEDQB_SIZE)
		dir_c = -1;
	else if (size == 0) {
		fill = !fill;
		dir_c = 1;
	}
}

