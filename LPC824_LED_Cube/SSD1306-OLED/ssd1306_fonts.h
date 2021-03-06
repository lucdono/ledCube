/*
    MIT License

    Copyright (c) 2018, Alexey Dynda

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/
/**
 * @file ssd1306_fonts.h Fonts for monochrome/rgb oled display
 */

#ifndef SSD1306_FONTS_H
#define SSD1306_FONTS_H

#include <stdint.h>

#define	FONT_PREAMBLE	4

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup LCD_FONTS FONTS: Supported LCD fonts
 * @{
 */

/** Standard ASCII 6x8 Fixed Type font.*/
extern const uint8_t ssd1306xled_font6x8 [];

/** Standard ASCII 8x16 Fixed Type font */
extern const uint8_t ssd1306xled_font8x16[];

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif // SSD1306_FONTS_H
