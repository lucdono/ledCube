/******************************************************************************
 * Includes
 ******************************************************************************/
#include <math.h>

#include "cube.h"
#include "cube_math.h"
#include "cube_interface.h"
#include "adc.h"

/******************************************************************************
 * Defines
 ******************************************************************************/
#define AUDIO_FFT					0
#define AUDIO_VOLUME				1

#define ADC_12BIT_MIN_VALUE			2048U
#define ADC_12BIT_MAX_VALUE			4096U

/******************************************************************************
 * Internal Variables
 ******************************************************************************/
static uint8_t ready = false;
static uint8_t height[LEDQB_SIZE];
static uint16_t full_samples[ADC_BUFFER_DEPTH];

/******************************************************************************
 * Prototypes
 ******************************************************************************/
static uint8_t get_amplitude(void);

/******************************************************************************
 * API
 ******************************************************************************/
void ledQB_adc_sample(uint16_t *adc_samples) {
	if (!ready) {
		memcpy(full_samples, adc_samples, ADC_BUFFER_DEPTH);
		ready = true;
	}
}

/******************************************************************************
 * Internal Functions
 ******************************************************************************/
static uint8_t get_amplitude(void) {
	uint16_t signalMax = 0;
	uint16_t signalMin = ADC_12BIT_MIN_VALUE;
	uint16_t value = 0;

	uint16_t i = 0;

	for (i = 0; i < ADC_BUFFER_DEPTH; i++) {
		value = full_samples[i];
		if (value > signalMax) {
			signalMax = value;
		} else if (value < signalMin) {
			signalMin = value;
		}
	}
	return map((signalMax - signalMin), ADC_12BIT_MIN_VALUE,
			ADC_12BIT_MAX_VALUE, 0, LEDQB_SIZE);
}

/******************************************************************************
 * Functions
 ******************************************************************************/
void f_adc(uint16_t frame) {
	uint8_t y = 0;
	uint8_t z = 0;

	(void) frame;

	if (ready) {
		for (y = 0; y < LEDQB_SIZE; y++)
			height[y] = get_amplitude();

		ledQB_shiftXLayer(true);
		ledQB_clrXLayer(LEDQB_SIZE - 1);

		for (y = 0; y < LEDQB_SIZE; y++) {
			for (z = 0; z < height[y]; z++) {
				point_t point = { LEDQB_SIZE - 1, y, z, 1 };
				ledQB_point(point);
			}
		}
	}

	ready = false;
}

