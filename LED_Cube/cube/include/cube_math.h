/*
 * chMath.h
 *
 *  Created on: 20/mag/2015
 *      Author: Luca D'Onofrio
 */

#ifndef _CUBEMATH_H_
#define _CUBEMATH_H_

#include <stdint.h>

#define A_180	180
#define A_0	    0
#define A_90	90
#define A_360	360

#define CUBEMATH_SIN	10000

#define map(in, inMin, inMax, outMin, outMax) (((float)(in - inMin)) / ((float)(inMax - inMin)) * ((float)(outMax - outMin)) + (float)outMin)

extern int16_t sin_table[A_360];

uint16_t ledQB_sqrt(uint16_t a_nInput);

#endif /* _CUBEMATH_H_ */
