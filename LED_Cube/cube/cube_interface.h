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
#define ADC_BUFFER_DEPTH      	256

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

void ledQB_osal_sleep(uint8_t sleep_ms);
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
void ledQB_set_runMode(char *mode);

void ledQB_adc_sample(uint16_t *adc_samples);

#ifdef __cplusplus
}
#endif

#endif /* CUBE_INTERFACE_H_ */
