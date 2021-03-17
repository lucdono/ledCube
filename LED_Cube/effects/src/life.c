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
#define min(a,b) 			((a)<(b)?(a):(b))
#define max(a,b) 			((a)>(b)?(a):(b))

/******************************************************************************
 * Internal Variables
 ******************************************************************************/
static uint16_t population = 0;
static uint16_t generations = 0;
static uint8_t temp_buffer[LEDQB_SIZE][LEDQB_SIZE];

/******************************************************************************
 * Variables
 ******************************************************************************/
uint16_t max_population = 0;
uint16_t max_generations = 0;

/******************************************************************************
 * Internal Functions
 ******************************************************************************/
static uint8_t getCell(uint8_t x, uint8_t dx) {
	int8_t sum = x + dx;

	if (sum <= 0)
		return LEDQB_SIZE - 1;
	else if (sum >= LEDQB_SIZE)
		return 0;
	else
		return sum;
}

static uint8_t getNeighbors(uint8_t xc, uint8_t yc, uint8_t zc) {
	uint8_t n = 0;
	int8_t dx = 0;
	int8_t dy = 0;
	int8_t dz = 0;

	for (dx = -1; dx <= 1; dx++) {
		for (dy = -1; dy <= 1; dy++) {
			for (dz = -1; dz <= 1; dz++) {
				if (!(dy == 0 && dx == 0 && dz == 0)) { // Do not count centre
					n += isAlive(getCell(dx, xc), getCell(dy, yc), getCell(dz, zc));
				}
			}
		}
	}
	return n;
}

static uint16_t getPopulation(void) {
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

/******************************************************************************
 * Functions
 ******************************************************************************/
void f_life_init(void) {
	uint16_t i = 0;

	for (i = 0; i < SEEDS; i++) {
		point_t p = { rand() % LEDQB_SIZE, rand() % LEDQB_SIZE,
				rand() % LEDQB_SIZE, 1 };
		ledQB_point(p);
	}
	population = SEEDS;
	generations = 1;
}

void f_life(uint16_t frame) {
	(void) frame;

	uint8_t x = 0;
	uint8_t y = 0;
	uint8_t z = 0;

	memset(temp_buffer, 0, LEDQB_SIZE * LEDQB_SIZE * sizeof(uint8_t));

	for (x = 0; x < LEDQB_SIZE; x++) {
		for (y = 0; y < LEDQB_SIZE; y++) {
			for (z = 0; z < LEDQB_SIZE; z++) {
				uint8_t n = getNeighbors(x, y, z);
				point_t p = { x, y, z, 1 };

				if (n == ALIVE_N) {
					if (!isAlive(x, y, z)) {
						//A dead cell becomes alive if it has exactly LIVE neighbours
						p.color = 1;
					} else {
						//Live
					}
				} else if (n <= ALONE_N) {
					//Die for loneliness
					p.color = 0;
				} else if (n >= OVER_N) {
					// Die for over-population
					p.color = 0;
				}

				if (p.color != 0) {
					BIT_SET(temp_buffer[p.z][p.x], BIT(p.y));
				} else {
					BIT_CLEAR(temp_buffer[p.z][p.x], BIT(p.y));
				}
			}
		}
	}

	memset(frame_buffer, 0, LEDQB_SIZE * LEDQB_SIZE * sizeof(uint8_t));
	memcpy(frame_buffer, temp_buffer,
			LEDQB_SIZE * LEDQB_SIZE * sizeof(uint8_t));

	if (population == 0) {
		f_life_init();
		ledQB_Callback();
	} else {
		if (population > max_population)
			max_population = population;
		if (generations > max_generations)
			max_generations = generations;

		population = getPopulation();
		generations++;
	}
}
