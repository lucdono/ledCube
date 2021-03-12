/******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdlib.h>

#include "cube.h"
#include "plane_suspend.h"

/******************************************************************************
 * Internal Variables
 ******************************************************************************/
static bool down = true;
static bool pass = false;
static int8_t increment = -1;
static uint8_t layer[LEDQB_SIZE];
static point_t drops[SUSPEND_DROPS];

/******************************************************************************
 * Functions
 ******************************************************************************/
void f_plane_suspend_init(void) {
	down = true;
	pass = false;
	increment = -1;
}

void f_plane_suspend(uint16_t frame) {
	uint8_t i = 0;
	uint8_t plane = 0;

	frame = frame % LEDQB_SIZE;

	memset(layer, 0, LEDQB_SIZE * sizeof(uint8_t));
	memset(drops, 0, SUSPEND_DROPS * sizeof(point_t));

	if (pass == 0 && down == 1 && frame == 0) {
		ledQB_plane(XY, LEDQB_SIZE - 1);
		return;
	}

	if (pass == 0) {
		plane = LEDQB_SIZE - frame;
		increment = -1;
	} else if (frame <= LEDQB_SIZE - 1) {
		plane = frame;
		increment = 1;
	}

	if (down == 1) {
		/* Generate random points */
		for (i = 0; i < SUSPEND_DROPS; i++) {
			drops[i].x = rand() % LEDQB_SIZE;
			drops[i].y = rand() % LEDQB_SIZE;
		}

		/* Move the layer to the following plane */
		if (frame <= LEDQB_SIZE - 1) {
			memcpy(layer, ledQB_getZLayer(plane), LEDQB_SIZE * sizeof(uint8_t));
			ledQB_clrLayer(plane);
			ledQB_setZLayer(layer, plane + increment);
		}

		/* Set the generated points */
		for (i = 0; i < SUSPEND_DROPS; i++) {
			drops[i].color = 1;
			drops[i].z = plane;
			ledQB_point(drops[i]);
			drops[i].color = 0;
			drops[i].z = plane + increment;
			ledQB_point(drops[i]);
		}
	} else {
		/* Or the layer with the following plane */
		if (frame <= LEDQB_SIZE - 1) {
			memcpy(layer, ledQB_getZLayer(plane), LEDQB_SIZE * sizeof(uint8_t));
			ledQB_clrLayer(plane);
			ledQB_orLayer(layer, plane + increment);
		}
	}

	if (frame == LEDQB_SIZE - 1 && down == false)
		pass = !pass;

	if (frame == LEDQB_SIZE - 1)
		down = !down;
}

