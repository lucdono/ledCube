/******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdlib.h>

#include "cube.h"
#include "effects.h"
#include "font.h"
#include "write.h"

/******************************************************************************
 * Defines
 ******************************************************************************/
#define NOT_FLIP	 0
#define FLIP	     1

/******************************************************************************
 * Internal Variables
 ******************************************************************************/
static char buffer[MAX_STRING];
static uint8_t char_index = 0;
static uint8_t mode = MODE_SURROUND;

/******************************************************************************
 * Internal Functions
 ******************************************************************************/
static void put_char(char c, int8_t offset) {

	if (offset < 0)
		ledQB_char(ZX, LEDQB_SIZE - 1, c, FLIP, offset);
	else if (offset >= 0 && offset < LEDQB_SIZE - 1) {
		ledQB_char(ZX, LEDQB_SIZE - 1, c, FLIP, offset);
		ledQB_char(YZ, LEDQB_SIZE - 1, c, NOT_FLIP,
				(LEDQB_SIZE - 1) - offset);
	} else if (offset >= LEDQB_SIZE - 1 && offset < 2 * LEDQB_SIZE - 1) {
		ledQB_char(YZ, LEDQB_SIZE - 1, c, NOT_FLIP,
				(LEDQB_SIZE - 1) - offset);
		ledQB_char(ZX, 0, c, NOT_FLIP, 2 * (LEDQB_SIZE - 1) - offset);
	} else {
		ledQB_char(ZX, 0, c, NOT_FLIP, 2 * (LEDQB_SIZE - 1) - offset);
	}

}

/******************************************************************************
 * Functions
 ******************************************************************************/
void f_write_init(void) {
	memset(buffer, 0, MAX_STRING);
	memcpy(buffer, LEDQB_FULL_NAME, strlen(LEDQB_FULL_NAME));
	char_index = 0;
}

void f_write(uint16_t frame) {
	ledQB_terminate_effect(false);

	switch (mode) {
	case MODE_YZ: {
		uint8_t axis = (LEDQB_SIZE - 1) - frame % LEDQB_SIZE;

		ledQB_char(YZ, axis, buffer[char_index], 0, 0);

		if (axis == 0)
			char_index++;

		if (char_index >= strlen(buffer)) {
			char_index = 0;
			ledQB_terminate_effect(true);
			mode = MODE_SURROUND;
		}

	}
		break;
	case MODE_SURROUND: {
		uint8_t i = 0;

		for (i = char_index; i < strlen(buffer); i++) {
			int16_t f = frame - i * LEDQB_SIZE;
			int16_t offset = (f % (4 * LEDQB_SIZE * (i + 1))) - LEDQB_SIZE;

			if (offset < 3 * LEDQB_SIZE - 1)
				put_char(buffer[i], offset);
			else {
				char_index++;
				if (char_index >= strlen(buffer)) {
					char_index = 0;
					ledQB_reset_frame();
					ledQB_terminate_effect(true);
					mode = MODE_YZ;
					return;
				}
			}
		}
	}
		break;
	}
}

