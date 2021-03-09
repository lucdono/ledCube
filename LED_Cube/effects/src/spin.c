/******************************************************************************
 * Includes
 ******************************************************************************/
#include <math.h>

#include "cube.h"
#include "cube_math.h"
#include "spin.h"

/******************************************************************************
 * Internal Variables
 ******************************************************************************/
static float dis = 0;
static float dir_spin = 1;

/******************************************************************************
 * Functions
 ******************************************************************************/
void f_spin_init(void){
	dis = 0;
	dir_spin = 1;
}

void f_spin(uint16_t frame) {
	uint8_t x = 0;
	uint8_t y = 0;
	uint8_t z = 0;

	uint16_t angle = frame % A_360;

	dis = dis + (0.01 * dir_spin);
	if (dis < -1.5 || dis > 1.5) {
		dir_spin = dir_spin * (-1);
	}

	for (x = 0; x < LEDQB_SIZE; x++) {
		for (y = 0; y < LEDQB_SIZE; y++) {
			int16_t theta = map(y, 0, LEDQB_SIZE - 1, angle,
					(angle+ ((uint16_t) round(dis * A_90))% A_360));
			float sinTheta = (float) sin_table[theta % A_360] / CUBEMATH_SIN;
			float X = ((float) x - LEDQB_SIZE / 2) * sinTheta;
			for (z = 0; z < LEDQB_SIZE; z++) {
				float cosTheta = (float) sin_table[(theta + A_90) % A_360]
						/ CUBEMATH_SIN;

				float Z = ((float) z - LEDQB_SIZE / 2) * cosTheta;
				if (fabs(X - Z) < 0.7) {
					point_t point = { x, y, z, 1 };
					ledQB_point(point);
				}
			}
		}
	}
}
