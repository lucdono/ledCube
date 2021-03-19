/*
 * 3D LED CUBE SSD1306 Implementation
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

/******************************************************************************
 * Includes
 ******************************************************************************/
#include <string.h>

#include "ssd1306_oled.h"
#include "ssd1306_fonts.h"

/******************************************************************************
 * Internal Variables
 ******************************************************************************/

/*
 * Display buffer
 */
static uint8_t SSD1306_Buffer[(DISPLAY_WIDTH * DISPLAY_HEIGHT) / 8];

/*
 * Current font
 */
static const uint8_t *font = &ssd1306xled_font6x8[0];

/*
 * Pre-scroll commands
 */
static const uint8_t pre_scroll[] = {
		SSD1306_LEFT_HORIZONTAL_SCROLL, 0X00
};

/*
 * Post-scroll commands
 */
static const uint8_t post_scroll[] = {
		0X00, 0XFF,
		SSD1306_ACTIVATE_SCROLL
};

/*
 * Update commands
 */
static const uint8_t update[] = {
		0xb0,
		(((0 & 0xf0) >> 4) | 0x10),
		(0 & 0x0f) | 0x01,
};

/*
 * Reset commands
 */
static const uint8_t reset[] = {
		SSD1306_DISPLAYON,
		SSD1306_DISPLAYOFF,
		SSD1306_DISPLAYON, };

/*
 * SSD1306 128x64 configuration commands
 */
static const uint8_t configuration[] = {
	SSD1306_DISPLAYOFF,
	SSD1306_SETDISPLAYCLOCKDIV, 0x80,
	SSD1306_SETMULTIPLEX,
	DISPLAY_HEIGHT - 1,
	SSD1306_SETDISPLAYOFFSET, 0x0,
	SSD1306_SETSTARTLINE | 0x0,
	SSD1306_CHARGEPUMP, 0x14,
	SSD1306_MEMORYMODE, 0x00,
	SSD1306_SEGREMAP | 0x1,
	SSD1306_COMSCANDEC,
	SSD1306_SETCOMPINS, 0x12,
	SSD1306_SETCONTRAST, 0xCF,
	SSD1306_SETPRECHARGE, 0xF1,
	SSD1306_SETVCOMDETECT, 0x40,
	SSD1306_DISPLAYALLON_RESUME,
	SSD1306_NORMALDISPLAY,
	SSD1306_DEACTIVATE_SCROLL,
	SSD1306_COLUMNADDR, 0,
	DISPLAY_WIDTH - 1,
	SSD1306_PAGEADDR, 0,
	DISPLAY_HEIGHT / 8 - 1,
	SSD1306_DISPLAYON,
	SSD1306_SETLOWCOLUMN | 0x0,
	SSD1306_SETHIGHCOLUMN | 0x0,
	SSD1306_SETSTARTLINE | 0x0, };

/******************************************************************************
 * Internal Functions
 ******************************************************************************/
static void SSD1306_CommandList(const uint8_t *commands, uint8_t size) {
	for (uint8_t i = 0; i < size; i++) {
		SSD1306_Command(commands[i]);
	}
}

/******************************************************************************
 * Functions
 ******************************************************************************/
void Display_Reset(void) {
	SSD1306_CommandList(&reset[0], sizeof(reset));
}

void Display_Init(void) {
	Display_Reset();
	Display_Clear();

	SSD1306_CommandList(&configuration[0], sizeof(configuration));
}

void Display_Update(void) {
	SSD1306_CommandList(&update[0], sizeof(update));
	SSD1306_Data(&SSD1306_Buffer[0], sizeof(SSD1306_Buffer));
}

void Display_Clear(void) {
	memset(SSD1306_Buffer, 0, sizeof(SSD1306_Buffer));
}

void Display_SetFont(const uint8_t *new_font) {
	font = new_font;
}

void Display_Text(uint8_t x, uint8_t y, char *text) {
	uint8_t px, py, b;
	uint16_t start;
	uint8_t font_width = font[1];
	uint8_t font_height = font[2];
	uint8_t bytes = font_height / 8;

	if ((x < DISPLAY_WIDTH) && (y < DISPLAY_HEIGHT)) {
		for (uint8_t i = 0; text[i] != '\0'; i++) {
			char c = text[i] - ' '; // First printable char;

			start = ((uint8_t) c) * (font_width * bytes) + FONT_PREAMBLE;
			for (px = 0; px < font_width; px++) {
				uint16_t column = 0;
				for (b = 0; b < bytes; b++) {
					column |= (uint16_t) font[start + px + b * font_width]
							<< 8 * b;
				}
				for (py = font_height; py != 0; py--) {
					if ((column >> (py-1)) & 0x01) {
						Display_Pixel(x + px + i * font_width, y + py);
					}
				}
			}
		}
	}
}

void Display_Line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
	uint8_t dx = x1 > x2 ? (x1 - x2) : (x2 - x1);
	uint8_t dy = y1 > y2 ? (y1 - y2) : (y2 - y1);
	uint8_t err = 0;

	if (dy > dx) {
		for (; y1 <= y2; y1++) {
			err += dx;
			if (err >= dy) {
				err -= dy;
				x1 < x2 ? x1++ : x1--;
			}
			Display_Pixel(x1, y1);
		}
	} else {
		for (; x1 <= x2; x1++) {
			err += dy;
			if (err >= dx) {
				err -= dx;
				if (y1 < y2)
					y1++;
				else
					y1--;
			}
			Display_Pixel(x1, y1);
		}
	}
}

void Display_Pixel(uint8_t x, uint8_t y) {
	SSD1306_Buffer[x + (y / 8) * DISPLAY_WIDTH] |= (1 << (y & 7));
}

void Display_ScrollStop(void) {
	SSD1306_Command(SSD1306_DEACTIVATE_SCROLL);
}

void Display_ScrollLeft(uint8_t firstRow, uint8_t lastRow) {
	SSD1306_CommandList(&pre_scroll[0], sizeof(pre_scroll));
	SSD1306_Command(firstRow);
	SSD1306_Command(0x00);
	SSD1306_Command(lastRow);
	SSD1306_CommandList(&post_scroll[0], sizeof(post_scroll));
}


/*------------------------------------------------------------------------------
 Weak symbol definition for specific IO functions
 -----------------------------------------------------------------------------*/
__attribute__ ((weak)) void SSD1306_Command(uint8_t command) {}
__attribute__ ((weak)) void SSD1306_Data(uint8_t *data, size_t size) {}
