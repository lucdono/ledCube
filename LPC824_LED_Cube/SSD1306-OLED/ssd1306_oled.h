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

#ifndef SSD1306_OLED_H_
#define SSD1306_OLED_H_

/******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdint.h>
#include <stdbool.h>

/******************************************************************************
 * Defines
 ******************************************************************************/
#define DISPLAY_WIDTH 									128
#define DISPLAY_HEIGHT 									64

#define SSD1306_COMMAND 								(0x00)
#define SSD1306_DATA 									(0x40)

#define SSD1306_SETCONTRAST 							(0x81)
#define SSD1306_DISPLAYALLON_RESUME 					(0xA4)
#define SSD1306_DISPLAYALLON 							(0xA5)
#define SSD1306_NORMALDISPLAY 							(0xA6)
#define SSD1306_INVERTDISPLAY 							(0xA7)
#define SSD1306_DISPLAYOFF 								(0xAE)
#define SSD1306_DISPLAYON 								(0xAF)
#define SSD1306_SETDISPLAYOFFSET 						(0xD3)
#define SSD1306_SETCOMPINS 								(0xDA)
#define SSD1306_SETVCOMDETECT 							(0xDB)
#define SSD1306_SETDISPLAYCLOCKDIV 						(0xD5)
#define SSD1306_SETPRECHARGE 							(0xD9)
#define SSD1306_SETMULTIPLEX 							(0xA8)
#define SSD1306_SETLOWCOLUMN 							(0x00)
#define SSD1306_SETHIGHCOLUMN 							(0x10)
#define SSD1306_SETSTARTLINE 							(0x40)
#define SSD1306_MEMORYMODE 								(0x20)
#define SSD1306_COLUMNADDR 								(0x21)
#define SSD1306_PAGEADDR   								(0x22)
#define SSD1306_COMSCANINC 								(0xC0)
#define SSD1306_COMSCANDEC 								(0xC8)
#define SSD1306_SEGREMAP 								(0xA0)
#define SSD1306_CHARGEPUMP 								(0x8D)
#define SSD1306_EXTERNALVCC 							(0x01)
#define SSD1306_SWITCHCAPVCC 							(0x02)
#define SSD1306_ACTIVATE_SCROLL 						(0x2F)
#define SSD1306_DEACTIVATE_SCROLL 						(0x2E)
#define SSD1306_SET_VERTICAL_SCROLL_AREA 				(0xA3)
#define SSD1306_RIGHT_HORIZONTAL_SCROLL 				(0x26)
#define SSD1306_LEFT_HORIZONTAL_SCROLL 					(0x27)
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 	(0x29)
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 	(0x2A)

#define SSD1306_7BITS_ADDRESS							(0x3C)

/******************************************************************************
 * Prototypes
 ******************************************************************************/

void Display_Init(void);
void Display_Update(void);
void Display_Clear(void);
void Display_SetFont(const uint8_t *font);
void Display_Text(uint8_t x, uint8_t y, char* text);
void Display_Pixel(uint8_t x, uint8_t y);
void Display_Line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) ;
void Display_ScrollLeft(uint8_t firstRow, uint8_t lastRow);
void Display_ScrollStop(void);

void SSD1306_Command(uint8_t command);
void SSD1306_Data(uint8_t *data, size_t size);

#endif /* SSD1306_OLED_H_ */
