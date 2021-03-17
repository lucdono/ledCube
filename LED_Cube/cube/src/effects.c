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
	{ "Text",    true,    1000,   60,   1,    true,   f_write_init,         f_write         },
	{ "Ripple",  true,    10000,  20,   10,   true,   NULL,                 f_ripple        },
	{ "Wave",    true,    10000,  25,   20,   true,   NULL,                 f_wave          },
	{ "Sphere",  true,    10000,  50,   -1,   true,   f_spheres_init,       f_spheres       },
	{ "Cubes",   true,    10000,  40,   1,    true,   f_cubes_init,         f_cubes         },
	{ "Rain",    true,    10000,  60,   1,    false,  NULL,                 f_rain          },
	{ "Move" ,   true,    10000,  50,   1,    true,   f_cubes_init,         f_cubes_move    },
	{ "Spiral",  true,    10000,  25,   15,   true,   NULL,                 f_spiral        },
	{ "Spin",    true,    10000,  10,   10,   true,   NULL,                 f_spin          },
	{ "Boing",   true,    10000,  50,   1,    true,   NULL,                 f_plane_boing   },
	{ "Suspend", true,    10000,  100,  1,    false,  f_plane_suspend_init, f_plane_suspend },
	{ "Life",    true,    10000,  100,  1,    false,  f_life_init,          f_life          },
	{ "Pyramid", true,    10000,  25,   1,    false,  f_pyramid_init,       f_pyramid       },
	{ "Volume",  true,    10000,  5,    -1,   false,  f_adc_init,           f_adc           },
	{ "",        false,   0,      0,    0,    false,  NULL,                 NULL            }
};

static effect_t *currentEffect;
static uint16_t frame = 0;
static bool terminate = true;
static bool quit = false;
static char run_mode[EFFECT_NAME];

/******************************************************************************
 * Defines
 ******************************************************************************/
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
