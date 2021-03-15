/******************************************************************************
 * Includes
 ******************************************************************************/
#include "cube.h"
#include "cubes.h"

/******************************************************************************
 * Defines
 ******************************************************************************/
#define EDGES		12
#define VERTEX		 8
#define AXES		 9

#define increment(frame, dir)	(dir*(LEDQB_SIZE-size) + (dir==0?1:-1)*(frame % (LEDQB_SIZE-size)))

/******************************************************************************
 * Internal Variables
 ******************************************************************************/
static int8_t dir_c = 1;
static uint8_t counter = 0;
static uint8_t size = 0;
static uint8_t fill = 0;
static uint8_t axis = 0;
static uint8_t s0 = 0;
static uint8_t s1 = 0;
static uint8_t s2 = 0;
static bool dirX = false;
static bool dirY = false;
static bool dirZ = false;

static line_t lines[EDGES] = {
		{ { 0, 0, 0 }, { 1, 0, 0 }, 1 },
		{ { 1, 0, 0 }, { 1, 1, 0 }, 1 },
		{ { 1, 1, 0 }, { 0, 1, 0 }, 1 },
		{ { 0, 1, 0 }, { 0, 0, 0 }, 1 },
		{ { 0, 0, 1 }, { 1, 0, 1 }, 1 },
		{ { 1, 0, 1 }, { 1, 1, 1 }, 1 },
		{ { 1, 1, 1 }, { 0, 1, 1 }, 1 },
		{ { 0, 1, 1 }, { 0, 0, 1 }, 1 },
		{ { 0, 0, 0 }, { 0, 0, 1 }, 1 },
		{ { 1, 0, 0 }, { 1, 0, 1 }, 1 },
		{ { 1, 1, 0 }, { 1, 1, 1 }, 1 },
		{ { 0, 1, 0 }, { 0, 1, 1 }, 1 },
};

/******************************************************************************
 * Functions
 ******************************************************************************/
static bool direction(uint8_t s, bool* dir){
	bool changed = false;

	if (s == LEDQB_SIZE-size-1 && *dir == false) {
		*dir = true;
		changed = true;
	} else if (s == 1 && *dir == true){
		*dir = false;
		changed = true;
	}
	return changed;
}

static void fill_cube(uint8_t s0, uint8_t s1, uint8_t s2, uint8_t b0, uint8_t b1,
		uint8_t b2, uint8_t size) {
	uint8_t x = 0;
	uint8_t y = 0;
	uint8_t z = 0;

	for (x = 0; x <= size; x++) {
		for (y = 0; y <= size; y++) {
			for (z = 0; z <= size; z++) {
				uint8_t X = b0 * (LEDQB_SIZE-1) + (b0 == 0 ? 1 : -1) * x;
				uint8_t Y = b1 * (LEDQB_SIZE-1) + (b1 == 0 ? 1 : -1) * y;
				uint8_t Z = b2 * (LEDQB_SIZE-1) + (b2 == 0 ? 1 : -1) * z;
				point_t point = { s0+X, s1+Y, s2+Z, 1 };
				ledQB_point(point);
			}
		}
	}
}

void f_cubes_init(void) {
	dir_c = 1;
	size = 0;
	fill = 0;
	s0 = 0;
	s1 = 0;
	s2 = 0;
	dirX = false;
	dirY = false;
	dirZ = false;
	axis = 0;
}

void f_cubes(uint16_t frame) {
	line_t line;
	uint8_t i = 0;

	size = size + dir_c;

	if (!fill) {
		for (i = 0; i < EDGES; i++) {
			line.start.x = size * lines[i].start.x;
			line.start.y = size * lines[i].start.y;
			line.start.z = size * lines[i].start.z;
			line.end.x = size * lines[i].end.x;
			line.end.y = size * lines[i].end.y;
			line.end.z = size * lines[i].end.z;
			line.color = 1;

			ledQB_line(line);
		}
	} else {
		uint8_t b0 = (0x01 & (counter % VERTEX)) >> 0;
		uint8_t b1 = (0x02 & (counter % VERTEX)) >> 1;
		uint8_t b2 = (0x04 & (counter % VERTEX)) >> 2;
		fill_cube(0, 0, 0, b0, b1, b2, size);
	}

	if (size == LEDQB_SIZE)
		dir_c = -1;
	else if (size == 0) {
		fill = !fill;
		dir_c = 1;
		counter++;
	}
}

void f_cubes_move(uint16_t frame) {
	size  = 2;
	bool changed = false;

	switch(axis){
	case 0:
	case 5:
		s0 = increment(frame, dirX);
		s1 = 0;
		s2 = axis == 0 ? 0 : LEDQB_SIZE-size-1;
		break;
	case 1:
	case 6:
		s1 = increment(frame, dirY);
		s0 = LEDQB_SIZE-size-1;
		s2 = axis == 1 ? 0 : LEDQB_SIZE-size-1;
		break;
	case 2:
	case 7:
		s0 = increment(frame, dirX);
		s1 = LEDQB_SIZE-size-1;
		s2 = axis == 2 ? 0 : LEDQB_SIZE-size-1;
		break;
	case 3:
	case 8:
		s0 = 0;
		s1 = increment(frame, dirY);
		s2 = axis == 3 ? 0 : LEDQB_SIZE-size-1;
		break;
	case 4:
	case 9:
		s0 = 0;
		s1 = 0;
		s2 = increment(frame, dirZ);
		break;
	}

	fill_cube(s0, s1, s2, 0, 0, 0, size);

	switch(axis){
	case 0:	/* Bottom Right layer */
	case 5:	/* Top Right layer */
	case 2: /* Bottom Left layer */
	case 7:	/* Top Left layer */
		changed = direction(s0, &dirX);
		break;
	case 1: /* Bottom Front layer */
	case 6: /* Top Front layer */
	case 3: /* Bottom Back layer */
	case 8: /* Top Back layer */
		changed = direction(s1, &dirY);
		break;
	case 4:	/* Going up */
	case 9:	/* Going down */
		changed = direction(s2, &dirZ);
		break;
	}

	if(changed){
		if(axis == AXES)
			f_cubes_init();
		else
			axis++;
	}
}

