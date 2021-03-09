/******************************************************************************
 * Includes
 ******************************************************************************/
#include "cube.h"
#include "cubes.h"

/******************************************************************************
 * Internal Variables
 ******************************************************************************/
static int8_t dir_c = 1;
static uint8_t size = 0;
static uint8_t fill = 0;

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

	uint8_t i = 0;
	line_t line;
	uint8_t x = 0;
	uint8_t y = 0;
	uint8_t z = 0;

	line.color = 1;

	size = size + dir_c;

	if (!fill) {
		for (i = 0; i < 12; i++) {
			switch (i) {
			case 0:
				line.start.x = 0;
				line.start.y = 0;
				line.start.z = 0;
				line.end.x = size;
				line.end.y = 0;
				line.end.z = 0;
				break;
			case 1:
				line.start.x = size;
				line.start.y = 0;
				line.start.z = 0;
				line.end.x = size;
				line.end.y = size;
				line.end.z = 0;
				break;
			case 2:
				line.start.x = size;
				line.start.y = size;
				line.start.z = 0;
				line.end.x = 0;
				line.end.y = size;
				line.end.z = 0;
				break;
			case 3:
				line.start.x = 0;
				line.start.y = size;
				line.start.z = 0;
				line.end.x = 0;
				line.end.y = 0;
				line.end.z = 0;
				break;
			case 4:
				line.start.x = 0;
				line.start.y = 0;
				line.start.z = size;
				line.end.x = size;
				line.end.y = 0;
				line.end.z = size;
				break;
			case 5:
				line.start.x = size;
				line.start.y = 0;
				line.start.z = size;
				line.end.x = size;
				line.end.y = size;
				line.end.z = size;
				break;
			case 6:
				line.start.x = size;
				line.start.y = size;
				line.start.z = size;
				line.end.x = 0;
				line.end.y = size;
				line.end.z = size;
				break;
			case 7:
				line.start.x = 0;
				line.start.y = size;
				line.start.z = size;
				line.end.x = 0;
				line.end.y = 0;
				line.end.z = size;
				break;
			case 8:
				line.start.x = 0;
				line.start.y = 0;
				line.start.z = 0;
				line.end.x = 0;
				line.end.y = 0;
				line.end.z = size;
				break;
			case 9:
				line.start.x = size;
				line.start.y = 0;
				line.start.z = 0;
				line.end.x = size;
				line.end.y = 0;
				line.end.z = size;
				break;
			case 10:
				line.start.x = size;
				line.start.y = size;
				line.start.z = 0;
				line.end.x = size;
				line.end.y = size;
				line.end.z = size;
				break;
			case 11:
				line.start.x = 0;
				line.start.y = size;
				line.start.z = 0;
				line.end.x = 0;
				line.end.y = size;
				line.end.z = size;
				break;
			}
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

	if (size == LEDQB_SIZE - 1)
		dir_c = -1;
	else if (size == 0) {
		fill = !fill;
		dir_c = 1;
	}
}

