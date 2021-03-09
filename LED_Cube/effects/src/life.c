/******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdlib.h>

#include "cube.h"
#include "life.h"
#include "bitfields.h"

/******************************************************************************
 * Defines
 ******************************************************************************/
#define isAlive(x,y,z)		ledQB_getPoint(x,y,z)
#define allDead()			ledQB_cleared()
#define set(x,y,z)			BIT_SET(cube[z][x], BIT_(y))
#define clear(x,y,z)		BIT_CLEAR(cube[z][x], BIT_(y))

/******************************************************************************
 * Internal Variables
 ******************************************************************************/
static point_t seeds[SEEDS];
static uint8_t cube[LEDQB_SIZE][LEDQB_SIZE];

/******************************************************************************
 * Internal Functions
 ******************************************************************************/
uint16_t getPopulation(void) {
	uint16_t p = 0;
	uint8_t x = 0;
	uint8_t y = 0;
	uint8_t z = 0;

	for (x = 0; x < LEDQB_SIZE; x++)
		for (y = 0; y < LEDQB_SIZE; y++)
			for (z = 0; z < LEDQB_SIZE; z++)
				p += isAlive(x, y, z);

	return p;
}

uint8_t getNeighbors(uint8_t xc, uint8_t yc, uint8_t zc) {
	uint8_t n = 0;

	uint8_t xs = (xc > 0 ? xc - 1 : 0);
	uint8_t ys = (yc > 0 ? yc - 1 : 0);
	uint8_t zs = (zc > 0 ? zc - 1 : 0);

	uint8_t xe = (xc < LEDQB_SIZE - 1 ? xc + 1 : LEDQB_SIZE - 1);
	uint8_t ye = (yc < LEDQB_SIZE - 1 ? yc + 1 : LEDQB_SIZE - 1);
	uint8_t ze = (zc < LEDQB_SIZE - 1 ? zc + 1 : LEDQB_SIZE - 1);

	uint8_t x = 0;
	uint8_t y = 0;
	uint8_t z = 0;

	n += isAlive(xs, y, z);
	n += isAlive(xs, ys, z);
	n += isAlive(xs, ye, z);
	n += isAlive(xs, y, zs);
	n += isAlive(xs, ys, zs);
	n += isAlive(xs, ye, zs);
	n += isAlive(xs, y, ze);
	n += isAlive(xs, ys, ze);
	n += isAlive(xs, ye, ze);

	n += isAlive(xe, y, z);
	n += isAlive(xe, ys, z);
	n += isAlive(xe, ye, z);
	n += isAlive(xe, y, zs);
	n += isAlive(xe, ys, zs);
	n += isAlive(xe, ye, zs);
	n += isAlive(xe, y, ze);
	n += isAlive(xe, ys, ze);
	n += isAlive(xe, ye, ze);

	n += isAlive(x, ys, z);
	n += isAlive(x, ys, ze);
	n += isAlive(x, ys, zs);
	n += isAlive(x, ye, z);
	n += isAlive(x, ye, ze);
	n += isAlive(x, ye, zs);
	n += isAlive(x, y, ze);
	n += isAlive(x, y, zs);

	return n;
}

/******************************************************************************
 * Functions
 ******************************************************************************/
void f_life_init(void) {
	uint8_t i = 0;

	for (i = 0; i < SEEDS; i++) {
		seeds[i].x = rand() % LEDQB_SIZE / 2;
		seeds[i].y = rand() % LEDQB_SIZE / 2;
		seeds[i].z = rand() % LEDQB_SIZE / 2;

		point_t p = { seeds[i].x, seeds[i].y, seeds[i].z, 1 };
		ledQB_point(p);
	}
}

void f_life(uint16_t frame) {
	(void)frame;

	ledQB_terminate_effect(false);

	uint8_t x = 0;
	uint8_t y = 0;
	uint8_t z = 0;

	for (x = 0; x < LEDQB_SIZE; x++) {
		for (y = 0; y < LEDQB_SIZE; y++) {
			for (z = 0; z < LEDQB_SIZE; z++) {
				uint8_t n = getNeighbors(x, y, z);
				if (n == ALIVE_N) {
					//A dead cell becomes alive if it has exactly LIVE neighbors
					if (!isAlive(x, y, z)) {
						set(x, y, z);
					} else {
						//Live
					}
				} else if (n <= ALONE_N) {
					//Die for loneliness
					clear(x, y, z);
				} else if (n >= OVER_N) {
					// Die for over-population
					clear(x, y, z);
				}
			}
		}
	}

	for (z = 0; z < LEDQB_SIZE; z++)
		ledQB_setZLayer(cube[z], z);

	if (allDead()){
		f_life_init();
		ledQB_terminate_effect(true);
	}
}
