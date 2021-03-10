/******************************************************************************
 * Includes
 ******************************************************************************/
#include "cube.h"
#include "cube_math.h"
#include "wave.h"

/******************************************************************************
 * Functions
 ******************************************************************************/
void f_wave(uint16_t frame) {
	uint8_t x = 0;
	uint8_t y = 0;
	uint8_t z = 0;

	for (x = 0; x < LEDQB_SIZE; x++) {
		int16_t theta = map(x, 0, LEDQB_SIZE - 1, (-A_90+frame), (A_90+frame));
		int16_t sinTheta = sin_table[(theta + A_180) % A_360];
		for (y = 0; y < LEDQB_SIZE; y++) {
			int16_t phi = map(y, 0, LEDQB_SIZE - 1, (-A_90+frame),
					(A_90+frame));
			int16_t cosPhi = sin_table[(phi + A_90 + A_180) % A_360];
			z = map((sinTheta + 2 * cosPhi), -CUBEMATH_SIN, CUBEMATH_SIN, 2,
					LEDQB_SIZE-2);
			point_t point = { x, y, z, 1 };
			ledQB_point(point);
		}
	}
}

