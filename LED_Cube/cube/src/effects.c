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

/******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdlib.h>

#include "effects.h"
#include "draw.h"

#include "ripple.h"
#include "wave.h"
#include "spheres.h"
#include "cubes.h"
#include "rain.h"
#include "spiral.h"
#include "spin.h"
#include "plane_boing.h"
#include "plane_suspend.h"
#include "life.h"
#include "pyramid.h"
#include "write.h"
#include "adc.h"

/******************************************************************************
 * Internal Variables
 ******************************************************************************/
static effect_t effects[] = {
	/* NAME      ENABLE   ELAPSE  WAIT  RATE  CLEAR   INIT                  RUN             */
#ifdef ENABLE_EFFECT_TEXT
	{ "Text",    true,    1000,   60,   1,    true,   f_write_init,         f_write         },
#endif
#ifdef ENABLE_EFFECT_RIPPLE
	{ "Ripple",  true,    10000,  20,   10,   true,   NULL,                 f_ripple        },
#endif
#ifdef ENABLE_EFFECT_WAVE
	{ "Wave",    true,    10000,  25,   20,   true,   NULL,                 f_wave          },
#endif
#ifdef ENABLE_EFFECT_SPHERE
	{ "Sphere",  true,    10000,  50,   -1,   true,   f_spheres_init,       f_spheres       },
#endif
#ifdef ENABLE_EFFECT_CUBES
	{ "Cubes",   true,    10000,  40,   1,    true,   f_cubes_init,         f_cubes         },
#endif
#ifdef ENABLE_EFFECT_RAIN
	{ "Rain",    true,    10000,  60,   1,    false,  NULL,                 f_rain          },
#endif
#ifdef ENABLE_EFFECT_MOVE
	{ "Move" ,   true,    10000,  50,   1,    true,   f_cubes_init,         f_cubes_move    },
#endif
#ifdef ENABLE_EFFECT_SPIRAL
	{ "Spiral",  true,    10000,  25,   15,   true,   NULL,                 f_spiral        },
#endif
#ifdef ENABLE_EFFECT_SPIN
	{ "Spin",    true,    10000,  10,   10,   true,   NULL,                 f_spin          },
#endif
#ifdef ENABLE_EFFECT_BOING
	{ "Boing",   true,    10000,  50,   1,    true,   NULL,                 f_plane_boing   },
#endif
#ifdef ENABLE_EFFECT_SUSPEND
	{ "Suspend", true,    10000,  100,  1,    false,  f_plane_suspend_init, f_plane_suspend },
#endif
#ifdef ENABLE_EFFECT_LIFE
	{ "Life",    true,    10000,  100,  1,    false,  f_life_init,          f_life          },
#endif
#ifdef ENABLE_EFFECT_PYRAMID
	{ "Pyramid", true,    10000,  25,   1,    false,  f_pyramid_init,       f_pyramid       },
#endif
#ifdef ENABLE_EFFECT_VOLUME
	{ "Volume",  true,    10000,  5,    -1,   false,  f_adc_init,           f_adc           },
#endif
	{ "",        false,   0,      0,    0,    false,  NULL,                 NULL            }
};

static effect_t *currentEffect;				/* Pointer to current effect */
static uint16_t frame = 0;					/* Current frame number */
static bool terminate = true;				/* Effect termination flag used to ignore effect time duration */
static bool quit = false;					/* Effect quit flag */
static char run_mode[EFFECT_NAME];			/* Current run mode */

/******************************************************************************
 * Defines
 ******************************************************************************/

/*
 * Clear the run mode
 */
#define ledQB_clr_runMode() 	(memset(run_mode, 0, EFFECT_NAME))

/******************************************************************************
 * Prototypes
 ******************************************************************************/
static void doEffect(effect_t *effect);
static void ledQB_update_effect(char *name);
static void ledQB_enable_all(void);
static void ledQB_update_runMode(void);

/******************************************************************************
 * API
 ******************************************************************************/
char* ledQB_get_currentEffect(void) {
	if (currentEffect != NULL)
		return currentEffect->name;
	return NULL;
}

char *ledQB_get_Effect(uint8_t index){
	if (effects[index].effectPtr == NULL) {
		return NULL;
	}
	return effects[index].name;
}

bool ledQB_effects(uint8_t index) {
	bool endEffects = false;

	if (effects[index].effectPtr == NULL) {
		endEffects = true;
		currentEffect = NULL;
	}

	if (effects[index].enable) {
		currentEffect = &effects[index];
		doEffect(currentEffect);
	}

	return endEffects;
}

void ledQB_terminate_effect(bool end) {
	terminate = end;
}

void ledQB_quit_effect() {
	quit = true;
}

void ledQB_set_runMode(char *mode) {
	ledQB_clr_runMode();
	memcpy(run_mode, mode, strlen(mode));
	ledQB_update_runMode();
}

char* ledQB_get_runMode(void) {
	return run_mode;
}

/******************************************************************************
 * Functions
 ******************************************************************************/
void ledQB_reset_frame(void) {
	frame = 0;
}

/******************************************************************************
 * Internal Functions
 ******************************************************************************/
static void doEffect(effect_t *effect) {
	uint32_t start = ledQB_osal_time_now();
	terminate = true;
	quit = false;
	frame = 0;

	if (effect->initPtr != NULL)
		effect->initPtr();

	while (effect->enable && !quit) {
		if (effect->alive_ms != ENDLESS_EFFECT) {
			uint32_t now = ledQB_osal_time_now();

			if (now - start > effect->alive_ms && terminate)
				break;
		}

		if (effect->clear)
			ledQB_clear();

		ledQB_osal_lock();
		effect->effectPtr(effect->frame_rate * (frame++));
		ledQB_osal_unlock();

		if (effect->frame_wait_ms != 0)
			ledQB_osal_sleep(effect->frame_wait_ms);
		else
			ledQB_osal_sleep(5);
	}
}

static void ledQB_update_effect(char *name) {
	uint8_t n = 0;

	while (true) {
		if (effects[n].effectPtr == NULL)
			break;
		if (strcmp(name, effects[n].name) == 0){
			effects[n].enable = true;
			currentEffect = &effects[n];
		}else
			effects[n].enable = false;
		n++;
	}
}

static void ledQB_enable_all(void) {
	uint8_t n = 0;
	char *mode = ledQB_get_runMode();

	if (strlen(mode) == 0) {
		while (true) {
			if (effects[n].effectPtr == NULL)
				break;
			effects[n].enable = true;
			n++;
		}
	}
}

static void ledQB_update_runMode(void) {
	char *mode = ledQB_get_runMode();

	if (strlen(mode) == 0)
		ledQB_enable_all();
	else
		ledQB_update_effect(mode);
}
