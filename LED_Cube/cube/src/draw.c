/******************************************************************************
 * Includes
 ******************************************************************************/
#include <string.h>

#include "bitfields.h"
#include "cube_math.h"
#include "font.h"
#include "draw.h"

/******************************************************************************
 * Defines
 ******************************************************************************/
#define LINE_FACTOR		8

/******************************************************************************
 * Functions
 ******************************************************************************/
void ledQB_point(point_t p) {
	if (ledQB_inRange(p)) {
		if (p.color != 0) {
			BIT_SET(frame_buffer[p.z][p.x], BIT(p.y));
		} else {
			BIT_CLEAR(frame_buffer[p.z][p.x], BIT(p.y));
		}
	}
}

void ledQB_shiftXLayer(uint8_t dir) {
	uint8_t x = 0;
	uint8_t z = 0;
	uint8_t offset = (dir ? 0 : 1);
	uint8_t limit = (dir ? LEDQB_SIZE - 1 : 1);

	for (x = offset; x < LEDQB_SIZE + (offset - 1); x++) {
		for (z = 0; z < LEDQB_SIZE; z++) {
			frame_buffer[z][x - offset] = frame_buffer[z][x + 1 - offset];
			if (x == limit)
				frame_buffer[z][limit] = 0;
		}
	}
}

void ledQB_clrXLayer(uint8_t index) {
	uint8_t z = 0;

	for (z = 0; z < LEDQB_SIZE; z++) {
		frame_buffer[z][index] = 0;
	}
}

void ledQB_orLayer(uint8_t *layer, uint8_t index) {
	uint8_t i = 0;

	for (i = 0; i < LEDQB_SIZE; i++)
		frame_buffer[index][i] = frame_buffer[index][i] | layer[i];
}

void ledQB_line(line_t l) {
	uint8_t t = 0;
	int8_t dir_vector[3] = { l.end.x - l.start.x, l.end.y - l.start.y, l.end.z
			- l.start.z };
	point_t p;

	p.color = l.color;

	for (t = 0; t <= LINE_FACTOR; t++) {
		p.x = (LINE_FACTOR * l.start.x + dir_vector[0] * t)/LINE_FACTOR;
		p.y = (LINE_FACTOR * l.start.y + dir_vector[1] * t)/LINE_FACTOR;
		p.z = (LINE_FACTOR * l.start.z + dir_vector[2] * t)/LINE_FACTOR;

		ledQB_point(p);
	}
}

void ledQB_plane(uint8_t plane, uint8_t axis) {
	uint8_t z = 0;
	uint64_t pattern;

	switch (plane) {
	case XY:
		pattern = 0xFF;
		memset(frame_buffer[axis], pattern, LEDQB_SIZE * sizeof(uint8_t));
		break;
	case ZX:
		pattern = BIT(axis) | BIT(axis) << 8 | BIT(axis) << 16 | BIT(axis) << 24;
		for(z = 0; z < LEDQB_SIZE; z++)
			memset(frame_buffer[z], pattern, LEDQB_SIZE * sizeof(uint8_t));
		break;
	case YZ:
		pattern = BF_MASK((uint64_t)(axis*LEDQB_SIZE), LEDQB_SIZE);
		for (z = 0; z < LEDQB_SIZE; z++)
			memcpy(frame_buffer[z], &pattern, LEDQB_SIZE * sizeof(uint8_t));
		break;
	}
}

color_t ledQB_getPoint(uint8_t x, uint8_t y, uint8_t z) {
	point_t p = { x, y, z, 0 };

	if (ledQB_inRange(p)) {
		return (BIT(y) & frame_buffer[z][x]);
	}
	return (color_t) 0;
}

uint8_t ledQB_cleared(void) {
	uint8_t i = 0;
	uint8_t *ptr = &frame_buffer[0][0];

	for (i = 0; i < LEDQB_SIZE*LEDQB_SIZE; i++) {
		if (*ptr++)
			return false;
	}

	return true;
}

void ledQB_char(plane_t plane, uint8_t axis, char c, uint8_t f, uint8_t offset) {
	uint8_t x = 0;
	uint8_t y = 0;
	uint8_t z = 0;

	c = c - ' '; // First printable char
	switch (plane) {
	case XY:
		for (y = 0; y < LEDQB_SIZE; y++)
			for (x = 0; x < LEDQB_SIZE; x++) {
				if ((fonts[c % ASCII_FONTS][x]) & (BIT(y))) {
					point_t point = { x, y, axis, 1 };
					ledQB_point(point);
				}
			}
		break;
	case ZX:
		for (z = 0; z < LEDQB_SIZE; z++)
			for (x = 0; x < LEDQB_SIZE; x++) {
				if ((fonts[c % ASCII_FONTS][z]) & (BIT(x))) {
					point_t point = { flip(f, x + offset), axis,
							(LEDQB_SIZE - 1) - z, 1 };
					ledQB_point(point);
				}
			}
		break;
	case YZ:
		for (y = 0; y < LEDQB_SIZE; y++) {
			for (z = 0; z < LEDQB_SIZE; z++)
				if ((fonts[c % ASCII_FONTS][z]) & (BIT(y))) {
					point_t point = { axis, flip(f, y + offset),
							(LEDQB_SIZE - 1) - z, 1 };
					ledQB_point(point);
				}
		}
		break;
	}
}

