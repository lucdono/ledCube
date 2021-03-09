/******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdlib.h>

#include "cube.h"
#include "cube_math.h"
#include "spheres.h"

/******************************************************************************
 * Internal Variables
 ******************************************************************************/
static int8_t dir_s = 1;
static uint8_t r = 0;
static uint8_t cx = (LEDQB_SIZE - 1) / 2;
static uint8_t cy = (LEDQB_SIZE - 1) / 2;
static uint8_t cz = (LEDQB_SIZE - 1) / 2;
static uint32_t start = 0;

/******************************************************************************
 * Functions
 ******************************************************************************/
void f_spheres_init(void) {
	dir_s = 1;
	r = 0;
	cx = (LEDQB_SIZE - 1) / 2;
	cy = (LEDQB_SIZE - 1) / 2;
	cz = (LEDQB_SIZE - 1) / 2;
	start = 0;
}

void f_spheres(uint16_t frame) {
	(void) frame;

	uint16_t theta = 0;
	uint16_t phi = 0;
	uint8_t x = 0;
	uint8_t y = 0;
	uint8_t z = 0;

	r = r + dir_s;

	for (phi = 0; phi < A_180; phi += SPHERE_STEP) {
		for (theta = 0; theta < A_360; theta += SPHERE_STEP) {
			float cosTheta = (float) sin_table[(theta + A_90) % A_360]
					/ CUBEMATH_SIN;
			float cosPhi = (float) sin_table[phi + A_90] / CUBEMATH_SIN;
			float sinPhi = (float) sin_table[phi] / CUBEMATH_SIN;
			float sinTheta = (float) sin_table[theta] / CUBEMATH_SIN;

			float radius = (float) dir_s * (float) r;

			x = cx + (int8_t) round(radius * cosTheta * sinPhi);
			y = cy + (int8_t) round(radius * sinTheta * sinPhi);
			z = cz + (int8_t) round(radius * cosPhi);

			point_t point = { x, y, z, 1 };
			ledQB_point(point);
		}
	}

	if (r == LEDQB_SIZE / 2)
		dir_s = -1;
	else if (r == 0)
		dir_s = 1;

	if (ledQB_osal_time_now() - start > 2000) {
		start = ledQB_osal_time_now();
		cx = rand() % (LEDQB_SIZE - 1);
		cy = rand() % (LEDQB_SIZE - 1);
		cz = rand() % (LEDQB_SIZE - 1);
	}
}

