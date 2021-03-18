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
#define ADC_BUFFER_DEPTH      	128
#define LEDQB_FULL_NAME      	"3D LED Cube v2"

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
void ledQB_board_init(void);
void ledQB_board_plane_select(uint8_t plane);
void ledQB_board_plane_unselect(uint8_t plane);
void ledQB_board_plane_send(uint8_t *data, uint8_t size);
uint32_t ledQB_board_plane_mask(uint8_t plane, uint32_t portStatus,
		uint8_t pin0, uint8_t pin1, uint8_t pin2, uint8_t pinEnable);

void ledQB_osal_sleep(uint32_t sleep_ms);
void ledQB_osal_lock(void);
void ledQB_osal_unlock(void);
void ledQB_osal_init(void);
uint32_t ledQB_osal_time_now(void);

void ledQB_init(void);
void ledQB_terminate_effect(bool end);
void ledQB_quit_effect();
bool ledQB_effects(uint8_t index);
void ledQB_refresh(void);

char *ledQB_get_runMode(void);
char *ledQB_get_currentEffect(void);
char *ledQB_get_Effect(uint8_t index);
void ledQB_set_runMode(char *mode);

void ledQB_adc_sample(uint16_t *adc_samples);
void ledQB_Callback(void);

#ifdef __cplusplus
}
#endif

#endif /* CUBE_INTERFACE_H_ */
