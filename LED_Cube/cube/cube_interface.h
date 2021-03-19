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

#ifndef CUBE_INTERFACE_H_
#define CUBE_INTERFACE_H_

/******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdint.h>
#include <stdbool.h>

/******************************************************************************
 * Defines
 ******************************************************************************/

/*
 * ADC sampling buffer depth
 * */
#define ADC_BUFFER_DEPTH      	128

/*
 * Cube name
 */
#define LEDQB_FULL_NAME      	"3D LED Cube v2"

/*
 * Effects enablement: on low Flash devices, choose the desired effects
 */
#define ENABLE_EFFECT_TEXT		1
#define ENABLE_EFFECT_RIPPLE	1
#define ENABLE_EFFECT_WAVE		1
#define ENABLE_EFFECT_SPHERE	1
#define ENABLE_EFFECT_CUBES		1
#define ENABLE_EFFECT_RAIN		1
#define ENABLE_EFFECT_MOVE		1
#define ENABLE_EFFECT_SPIRAL	1
#define ENABLE_EFFECT_SPIN		1
#define ENABLE_EFFECT_BOING		1
#define ENABLE_EFFECT_SUSPEND	1
#define ENABLE_EFFECT_LIFE		1
#define ENABLE_EFFECT_PYRAMID	1
#define ENABLE_EFFECT_VOLUME	1

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * API
 ******************************************************************************/

/*
 * Board specific IO initialisation function
 */
void ledQB_board_init(void);

/*
 * Board specific IO function to drive the 8-bit decoder selecting a specific plane
 */
void ledQB_board_plane_select(uint8_t plane);

/*
 * Board specific IO function to drive the 8-bit decoder un-selecting a specific plane
 */
void ledQB_board_plane_unselect(uint8_t plane);

/*
 * Board specific IO function to drive shift registers via SPI
 */
void ledQB_board_plane_send(uint8_t *data, uint8_t size);

/*
 * Board specific IO function to create the plane selection mask, reading the GPIO latch
 */
uint32_t ledQB_board_plane_mask(uint8_t plane, uint32_t portStatus,
		uint8_t pin0, uint8_t pin1, uint8_t pin2, uint8_t pinEnable);

/*
 * OS specific function: sleep
 */
void ledQB_osal_sleep(uint32_t sleep_ms);

/*
 * OS specific function: mutex lock to synchronise cube buffer read/write
 */
void ledQB_osal_lock(void);

/*
 * OS specific function: mutex unlock to synchronise cube buffer read/write
 */
void ledQB_osal_unlock(void);

/*
 * OS specific function: OS initialisations
 */
void ledQB_osal_init(void);

/*
 * OS specific function: OS system time
 */
uint32_t ledQB_osal_time_now(void);

/*
 * Cube initialisation
 */
void ledQB_init(void);

/*
 * Enable/Disable effect duration timer
 */
void ledQB_terminate_effect(bool end);

/*
 * Force an effect to quit
 */
void ledQB_quit_effect();

/*
 * Execute effect at index
 */
bool ledQB_effects(uint8_t index);

/*
 * Refresh cube LEDs
 */
void ledQB_refresh(void);

/*
 * Get current run mode
 */
char *ledQB_get_runMode(void);

/*
 * Get current effect name
 */
char *ledQB_get_currentEffect(void);

/*
 * Get effect name at index
 */
char *ledQB_get_Effect(uint8_t index);

/*
 * Change run mode
 */
void ledQB_set_runMode(char *mode);

/*
 * Push the ADC samples to the IO algorithm
 */
void ledQB_adc_sample(uint16_t *adc_samples);

/*
 * General effect callback implementation
 */
void ledQB_Callback(void);

#ifdef __cplusplus
}
#endif

#endif /* CUBE_INTERFACE_H_ */
