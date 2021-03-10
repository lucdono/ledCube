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
static uint8_t algorithm;

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
void f_volume_init(void) {
	algorithm = AUDIO_VOLUME;
}

void f_fft_init(void) {
	algorithm = AUDIO_FFT;
}

void f_adc(uint16_t frame) {
	uint8_t y = 0;
	uint8_t z = 0;

	(void) frame;

	if (ready) {
		switch (algorithm) {
		case AUDIO_FFT:
			break;
		case AUDIO_VOLUME:
			for (y = 0; y < LEDQB_SIZE; y++)
				height[y] = get_amplitude();
			break;
		}

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

#if AUDIO_FFT
#define SAMPLE_RATE_HZ 				20000
#endif

#if AUDIO_FFT
#define FFT_SIZE 			128
#define SPECTRUM_MIN_DB  	20.0f
#define SPECTRUM_MAX_DB 	60.0f

static float frequencyWindow[LEDQB_SIZE + 1];
static int16_t fft_samples[FFT_SIZE * 2];
#endif

#if AUDIO_FFT
// Compute the average magnitude of a target frequency window vs. all other frequencies.
float windowMean(int16_t lowBin, int16_t highBin) {
	uint16_t i = 0;
	float windowMean;

	windowMean = 0;
	// Notice the first magnitude bin is skipped because it represents the
	// average power of the signal.
	for (i = 1; i < FFT_SIZE / 2; ++i) {
		if (i >= lowBin && i <= highBin) {
			windowMean += (float) fft_samples[i];
		}
	}
	windowMean /= (float) (highBin - lowBin) + 1.0;
	return windowMean;
}

// Convert a frequency to the appropriate FFT bin it will fall within.
int16_t frequencyToBin(int16_t frequency) {
	float binFrequency = (float) (SAMPLE_RATE_HZ) / (float) FFT_SIZE;
	return (int16_t) (frequency / binFrequency);
}

uint8_t get_magnitude(uint8_t index) {
// Update each LED based on the intensity of the audio
// in the associated frequency window.
	float intensity = 0.0f;

	int16_t bin0 = frequencyToBin(frequencyWindow[index]);
	int16_t bin1 = frequencyToBin(frequencyWindow[index + 1]);

	intensity = windowMean(bin0, bin1);
// Convert intensity to decibels.
	intensity = 20.0 * log10(intensity);
// Scale the intensity and clamp between 0 and 1.0.
	intensity -= SPECTRUM_MIN_DB;
	intensity = intensity < 0.0 ? 0.0 : intensity;
	intensity /= (SPECTRUM_MAX_DB - SPECTRUM_MIN_DB);
	return map(intensity, 0.0, 1.0, 0, LEDQB_SIZE);
}
#else

#endif

//void f_fft_init(void) {
#if AUDIO_FFT
	// Set the frequency window values by evenly dividing the possible frequency
	// spectrum across the number of neo pixels.
	float windowSize = (SAMPLE_RATE_HZ / 2.0) / (float) LEDQB_SIZE;
	uint8_t i = 0;
	for (i = 0; i < LEDQB_SIZE + 1; ++i) {
		frequencyWindow[i] = i * windowSize;
	}
#endif
//}
//
//void f_fft(uint16_t frame) {
//	uint8_t x = 0;
//	uint8_t y = 0;
//	uint8_t z = 0;
//
//	(void) frame;
//
//	if (ready) {
//#if AUDIO_FFT
//		uint16_t i = 0, l = 0;
//		for (i = 0; i < FFT_SIZE; i += 2) {
//			fft_samples[i] = full_samples[l++] - ADC_12BIT_MIN_VALUE;
//			fft_samples[i + 1] = 0;
//		}
//
//		for (y = 0; y < LEDQB_SIZE; y++)
//			height[y] = get_magnitude(y);
//#else
//
//		for (y = 0; y < LEDQB_SIZE; y++)
//			height[y] = get_amplitude();
//#endif
//	}
//
//	ledQB_shiftXLayer(true);
//	ledQB_clrXLayer(LEDQB_SIZE - 1);
//
//	for (y = 0; y < LEDQB_SIZE; y++) {
//		for (z = 0; z < height[y]; z++) {
//			point_t point = { LEDQB_SIZE - 1, y, z, 1 };
//			ledQB_point(point);
//		}
//	}
//
//	ready = false;
//}
