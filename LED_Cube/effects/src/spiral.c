/******************************************************************************
 * Includes
 ******************************************************************************/
#include "cube.h"
#include "cube_math.h"
#include "spiral.h"

/******************************************************************************
 * Functions
 ******************************************************************************/
void f_spiral(uint16_t frame) {
	uint8_t x = 0;
	uint8_t y = 0;
	uint8_t z = 0;

	for (z = 0; z < LEDQB_SIZE; z++) {
		int16_t theta = map(z, 0, LEDQB_SIZE - 1, (-A_180 + frame),
				(A_180 + frame));
		int16_t X = map(sin_table[theta % A_360], -CUBEMATH_SIN, CUBEMATH_SIN, 0,
				LEDQB_SIZE);
		int16_t Y = map(sin_table[((uint16_t) round(theta + A_90)) % A_360],
				-CUBEMATH_SIN, CUBEMATH_SIN, 0, LEDQB_SIZE);
		for (x = 0; x < LEDQB_SIZE; x++) {
			for (y = 0; y < LEDQB_SIZE; y++) {
				if (((x == X) || (x == X - 1)) && (y == Y)) {
					point_t point = { x, y, z, 1 };
					ledQB_point(point);
				}
			}
		}
	}
}
