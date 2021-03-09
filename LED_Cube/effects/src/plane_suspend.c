/******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdlib.h>

#include "cube.h"
#include "plane_suspend.h"

/******************************************************************************
 * Internal Variables
 ******************************************************************************/
static int8_t suspend_d = 1;
static int8_t pass = 0;

/******************************************************************************
 * Functions
 ******************************************************************************/
void f_plane_suspend_init(void){
	suspend_d = 1;
	pass = 0;
}

void f_plane_suspend(uint16_t frame) {
	uint8_t i = 0;
	uint8_t layer[LEDQB_SIZE];
	point_t drops[SUSPEND_DROPS];

	frame = frame % LEDQB_SIZE;

	memset(layer, 0, LEDQB_SIZE * sizeof(uint8_t));

	if (pass == 0 && suspend_d == 1 && frame == 0) {
		ledQB_plane(XY, LEDQB_SIZE - 1);
		return;
	}

	if (suspend_d == 1) {
		for (i = 0; i < SUSPEND_DROPS; i++) {
			drops[i].x = rand() % LEDQB_SIZE;
			drops[i].y = rand() % LEDQB_SIZE;
		}

		if (pass == 0) {
			memcpy(layer, ledQB_getZLayer(LEDQB_SIZE - frame),
			LEDQB_SIZE * sizeof(uint8_t));
			ledQB_clrLayer(LEDQB_SIZE - frame);
			ledQB_setZLayer(layer, LEDQB_SIZE - frame - 1);
		} else {
			if (frame < LEDQB_SIZE - 1) {
				memcpy(layer, ledQB_getZLayer(frame),
				LEDQB_SIZE * sizeof(uint8_t));
				ledQB_clrLayer(frame);
				ledQB_setZLayer(layer, frame + 1);
			}
		}

		for (i = 0; i < SUSPEND_DROPS; i++) {
			if (pass == 0) {
				drops[i].color = 1;
				drops[i].z = LEDQB_SIZE - frame;
				ledQB_point(drops[i]);
				drops[i].color = 0;
				drops[i].z = LEDQB_SIZE - frame - 1;
				ledQB_point(drops[i]);
			} else {
				if (frame < LEDQB_SIZE - 1) {
					drops[i].color = 1;
					drops[i].z = frame;
					ledQB_point(drops[i]);
					drops[i].color = 0;
					drops[i].z = frame + 1;
					ledQB_point(drops[i]);
				}
			}
		}
	} else if (frame < LEDQB_SIZE && suspend_d == -1) {
		if (pass == 0 && frame > 0) {
			memcpy(layer, ledQB_getZLayer(LEDQB_SIZE - frame),
			LEDQB_SIZE * sizeof(uint8_t));
			ledQB_clrLayer(LEDQB_SIZE - frame);
			ledQB_orLayer(layer, LEDQB_SIZE - frame - 1);
		} else if (pass == 1 && frame < LEDQB_SIZE - 1) {
			memcpy(layer, ledQB_getZLayer(frame),
			LEDQB_SIZE * sizeof(uint8_t));
			ledQB_clrLayer(frame);
			ledQB_orLayer(layer, frame + 1);
		}
	}

	if (frame == LEDQB_SIZE - 1 && suspend_d == 1)
		suspend_d = -1;
	else if (frame == LEDQB_SIZE - 1 && suspend_d == -1) {
		suspend_d = 1;
		if (pass == 0)
			pass = 1;
		else
			pass = 0;
	}

}

