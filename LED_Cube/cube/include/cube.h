#ifndef _LEDQB_H_
#define _LEDQB_H_

/******************************************************************************
 * Includes
 ******************************************************************************/
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "draw.h"
#include "cube_interface.h"

/******************************************************************************
 * Defines
 ******************************************************************************/
#define _LEDQB_
#define LEDQB_SIZE    			8

/**
 * @brief   Full library name string.
 */
#define LEDQB_FULL_NAME      "8x8x8 3D-LED Cube v1.0"

/******************************************************************************
 * External Variables
 ******************************************************************************/
/*
 * Data structure for cube that allows double buffering but not the intensity
 * regulation.
 * X dim information is hidden into uint8_t
 */
extern uint8_t frame_buffer[LEDQB_SIZE][LEDQB_SIZE];

#endif /* _LEDQB_H_ */
