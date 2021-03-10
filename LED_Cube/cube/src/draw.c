/******************************************************************************
 * Includes
 ******************************************************************************/
#include <string.h>

#include "cube.h"
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
			BIT_SET(frame_buffer[p.z][p.x], BIT_(p.y));
		} else {
			BIT_CLEAR(frame_buffer[p.z][p.x], BIT_(p.y));
		}
	}
}

uint8_t* ledQB_getZLayer(uint8_t index) {
	return frame_buffer[index];
}

void ledQB_setZLayer(uint8_t *layer, uint8_t index) {
	memcpy(frame_buffer[index], layer, LEDQB_SIZE * sizeof(uint8_t));
}

void ledQB_shiftXLayer(uint8_t dir) {
	uint8_t x = 0;
	uint8_t z = 0;

	if (dir) {
		for (x = 0; x < LEDQB_SIZE - 1; x++) {
			for (z = 0; z < LEDQB_SIZE; z++) {
				frame_buffer[z][x] = frame_buffer[z][x + 1];
				if (x == LEDQB_SIZE - 1)
					frame_buffer[z][LEDQB_SIZE - 1] = 0;
			}
		}
	} else {
		for (x = 1; x < LEDQB_SIZE; x++) {
			for (z = 0; z < LEDQB_SIZE; z++) {
				frame_buffer[z][x - 1] = frame_buffer[z][x];
				if (x == 1)
					frame_buffer[z][0] = 0;
			}
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
	uint8_t temp[LEDQB_SIZE];

	memset(temp, 0, LEDQB_SIZE * sizeof(uint8_t));
	for (i = 0; i < LEDQB_SIZE; i++)
		temp[i] = frame_buffer[index][i] | layer[i];
	memcpy(frame_buffer[index], temp, LEDQB_SIZE * sizeof(uint8_t));
}

void ledQB_clrLayer(uint8_t index) {
	memset(frame_buffer[index], 0, LEDQB_SIZE * sizeof(uint8_t));
}

void ledQB_line(line_t l) {
	uint8_t t = 0;
	int8_t dir_vector[3] = { l.end.x - l.start.x, l.end.y - l.start.y, l.end.z
			- l.start.z };
	point_t p;

	p.color = l.color;

	for (t = 0; t <= LINE_FACTOR; t++) {
		p.x = map((LINE_FACTOR * l.start.x + dir_vector[0] * t), 0,
				LINE_FACTOR*(LEDQB_SIZE), 0, LEDQB_SIZE - 1);
		p.y = map((LINE_FACTOR * l.start.y + dir_vector[1] * t), 0,
				LINE_FACTOR*(LEDQB_SIZE), 0, LEDQB_SIZE - 1);
		p.z = map((LINE_FACTOR * l.start.z + dir_vector[2] * t), 0,
				LINE_FACTOR*(LEDQB_SIZE), 0, LEDQB_SIZE - 1);

		ledQB_point(p);
	}
}

void ledQB_plane(uint8_t plane, uint8_t axis) {
	uint8_t x = 0;
	uint8_t y = 0;
	uint8_t z = 0;

	switch (plane) {
	case XY:
		for (y = 0; y < LEDQB_SIZE; y++)
			for (x = 0; x < LEDQB_SIZE; x++) {
				point_t point = { x, y, axis, 1 };
				ledQB_point(point);
			}
		break;
	case ZX:
		for (z = 0; z < LEDQB_SIZE; z++)
			for (x = 0; x < LEDQB_SIZE; x++) {
				point_t point = { x, axis, z, 1 };
				ledQB_point(point);
			}
		break;
	case YZ:
		for (z = 0; z < LEDQB_SIZE; z++)
			for (y = 0; y < LEDQB_SIZE; y++) {
				point_t point = { axis, y, z, 1 };
				ledQB_point(point);
			}
		break;
	}
}

void ledQB_clear(void) {
	memset(frame_buffer, 0, LEDQB_SIZE * LEDQB_SIZE * sizeof(uint8_t));
}

void ledQB_full(void) {
	memset(frame_buffer, 0xFF, LEDQB_SIZE * LEDQB_SIZE * sizeof(uint8_t));
}

color_t ledQB_getPoint(uint8_t x, uint8_t y, uint8_t z) {
	point_t p = { x, y, z, 0 };

	if (ledQB_inRange(p)) {
		if (BIT_(y) & frame_buffer[z][x])
			return (color_t) 1;
	}
	return (color_t) 0;
}

uint8_t ledQB_cleared(void) {
	uint8_t x = 0;
	uint8_t y = 0;
	uint8_t z = 0;

	for (x = 0; x < LEDQB_SIZE; x++) {
		for (y = 0; y < LEDQB_SIZE; y++) {
			for (z = 0; z < LEDQB_SIZE; z++) {
				if (ledQB_getPoint(x, y, z))
					return false;
			}
		}
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
				if ((fonts[c % ASCII_FONTS][x]) & (BIT_(y))) {
					point_t point = { x, y, axis, 1 };
					ledQB_point(point);
				}
			}
		break;
	case ZX:
		for (z = 0; z < LEDQB_SIZE; z++)
			for (x = 0; x < LEDQB_SIZE; x++) {
				if ((fonts[c % ASCII_FONTS][z]) & (BIT_(x))) {
					point_t point = { flip(f, x + offset), axis,
							(LEDQB_SIZE - 1) - z, 1 };
					ledQB_point(point);
				}
			}
		break;
	case YZ:
		for (y = 0; y < LEDQB_SIZE; y++) {
			for (z = 0; z < LEDQB_SIZE; z++)
				if ((fonts[c % ASCII_FONTS][z]) & (BIT_(y))) {
					point_t point = { axis, flip(f, y + offset),
							(LEDQB_SIZE - 1) - z, 1 };
					ledQB_point(point);
				}
		}
		break;
	}
}

