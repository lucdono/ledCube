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
 * Internal Functions
 ******************************************************************************/
uint8_t getNeighbors(uint8_t xc, uint8_t yc, uint8_t zc) {
	uint8_t n = 0;

	uint8_t x = 0;
	uint8_t y = 0;
	uint8_t z = 0;

	uint8_t xs = (xc != 0 ? xc - 1 : LEDQB_SIZE - 1);
	uint8_t ys = (yc != 0 ? yc - 1 : LEDQB_SIZE - 1);
	uint8_t zs = (zc != 0 ? zc - 1 : LEDQB_SIZE - 1);

	uint8_t xe = (xc == LEDQB_SIZE - 1 ? 0 : xc + 1);
	uint8_t ye = (yc == LEDQB_SIZE - 1 ? 0 : yc + 1);
	uint8_t ze = (zc == LEDQB_SIZE - 1 ? 0 : zc + 1);

	for (x = min(xs, xe); x <= max(xs, xe); (xs < xe ? x++ : x--)) {
		for (y = min(ys, ye); y <= max(ys, ye); (ys < ye ? y++ : y--)) {
			for (z = min(zs, ze); z <= max(zs, ze); (zs < ze ? z++ : z--)) {
				n += isAlive(x, y, z);
			}
		}
	}

	return n - 1; // Do not count self
}

/******************************************************************************
 * Functions
 ******************************************************************************/
void f_life_init(void) {
	uint16_t i = 0;

	for (i = 0; i < SEEDS; i++) {
		point_t p = { rand() % LEDQB_SIZE, rand() % LEDQB_SIZE, rand() % LEDQB_SIZE, 1 };
		ledQB_point(p);
	}
}

void f_life(uint16_t frame) {
	(void) frame;

	uint8_t x = 0;
	uint8_t y = 0;
	uint8_t z = 0;

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

				ledQB_point(p);
			}
		}
	}

	if (allDead()) {
		f_life_init();
	}
}
