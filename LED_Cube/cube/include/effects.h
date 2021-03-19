/*
 * 3D LED CUBE
 *
 * Copyright (C) 2021 Luca D'Onofrio.
 *
 * This file is part of LEDCube Project
 *
 * LEDCube is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * LEDCube is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef INCLUDE_EFFECTS_H_
#define INCLUDE_EFFECTS_H_

/******************************************************************************
 * Includes
 ******************************************************************************/
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "cube_interface.h"

/******************************************************************************
 * Defines
 ******************************************************************************/
#define EFFECT_NAME					10
#define ENDLESS_EFFECT		0xFFFFFFFF
#define DEFAULT_RUN_MODE			""

/******************************************************************************
 * Types
 ******************************************************************************/

/*
 * Light effect description structure
 */
typedef struct {
	char name[EFFECT_NAME];					/* Name */
	bool enable;							/* Enable/Disable effect */
	uint16_t alive_ms;						/* Duration */
	uint8_t frame_wait_ms;					/* Waiting time between frames */
	uint8_t frame_rate;						/* Frames increment */
	bool clear;								/* Clear the cube after each frame */
	void (*initPtr)(void);					/* Effect initialisation function */
	void (*effectPtr)(uint16_t frame);		/* Effect algorithm */
} effect_t;

/******************************************************************************
 * Prototypes
 ******************************************************************************/

/*
 * Reset the frame
 */
void ledQB_reset_frame(void);

#endif /* INCLUDE_EFFECTS_H_ */
