/******************************************************************************
 * Includes
 ******************************************************************************/
#include "cube.h"
#include "plane_boing.h"

/******************************************************************************
 * Functions
 ******************************************************************************/
void f_plane_boing(uint16_t frame) {
	uint8_t axis = 0;

	frame = frame % 48;
	if (frame < 16) {
		if (frame < LEDQB_SIZE)
			axis = frame;
		else
			axis = 16 - frame - 1;

		ledQB_plane(ZX, axis);
	} else if (frame >= 16 && frame < 32) {
		if (frame - 16 < LEDQB_SIZE)
			axis = frame - 16;
		else
			axis = 32 - frame - 1;
		ledQB_plane(XY, axis);
	} else {
		if (frame - 32 < LEDQB_SIZE)
			axis = frame - 32;
		else
			axis = 48 - frame - 1;

		ledQB_plane(YZ, axis);
	}
}

