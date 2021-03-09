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
typedef struct {
	char name[EFFECT_NAME];
	bool enable;
	uint16_t alive_ms;
	uint8_t frame_wait_ms;
	uint8_t frame_rate;
	bool clear;
	void (*initPtr)(void);
	void (*effectPtr)(uint16_t frame);
} effect_t;

/******************************************************************************
 * Prototypes
 ******************************************************************************/
void ledQB_reset_frame(void);

#endif /* INCLUDE_EFFECTS_H_ */
