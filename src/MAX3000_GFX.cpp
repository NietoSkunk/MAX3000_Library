/**
 * @file MAX3000_GFX.cpp
 *
 * Wrapper around MAX3000_Lib for use with the Adafruit GFX library.
 *
 * For use with https://github.com/NietoSkunk/FlippyDriver MAX3000 Driver.
 *
 * This library depends on https://github.com/adafruit/Adafruit-GFX-Library
 * being present on your system. Please make sure you have installed the latest
 * version before using this library.
 *
 * Derived from Adafruit_SSD1306: https://github.com/adafruit/Adafruit_SSD1306
 * Originally licensed under BSD License, with the following text:
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * Written by Limor Fried/Ladyada for Adafruit Industries, with
 * contributions from the open source community.
 *
 * BSD license, all text above, and the splash screen header file,
 * must be included in any redistribution.
 */

#include <MAX3000_GFX.h>

#ifdef __AVR__
#include <avr/pgmspace.h>
#elif defined(ESP8266) || defined(ESP32) || defined(ARDUINO_ARCH_RP2040)
#include <pgmspace.h>
#else
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))    ///< PROGMEM workaround for non-AVR
#endif

#define MAX3000_swap(a, b) (((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b)))    ///< No-temp-var swap operation

MAX3000_GFX::MAX3000_GFX(const MAX3000_Config & config_)
    : Adafruit_GFX(config_.width, config_.height), MAX3000_Base(config_) {
}

MAX3000_GFX::~MAX3000_GFX(void) {
}

void MAX3000_GFX::drawPixel(int16_t x, int16_t y, uint16_t color) {
    MAX3000_Base::drawPixel(x, y, color);
}

void MAX3000_GFX::invertDisplay(bool i) {
    MAX3000_Base::invertDisplay(i);
}

void MAX3000_GFX::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
    bool bSwap = false;
    switch(getRotation()) {
        case 1:
            // 90 degree rotation, swap x & y for rotation, then invert x
            bSwap = true;
            MAX3000_swap(x, y);
            x = WIDTH - x - 1;
            break;
        case 2:
            // 180 degree rotation, invert x and y, then shift y around for height.
            x = WIDTH - x - 1;
            y = HEIGHT - y - 1;
            x -= (w - 1);
            break;
        case 3:
            // 270 degree rotation, swap x & y for rotation,
            // then invert y and adjust y for w (not to become h)
            bSwap = true;
            MAX3000_swap(x, y);
            y = HEIGHT - y - 1;
            y -= (w - 1);
            break;
    }

    if(bSwap)
        drawFastVLineInternal(x, y, w, color);
    else
        drawFastHLineInternal(x, y, w, color);
}

void MAX3000_GFX::drawFastHLineInternal(int16_t x, int16_t y, int16_t w, uint16_t color) {
    if((y >= 0) && (y < HEIGHT)) {    // Y coord in bounds?
        if(x < 0) {                   // Clip left
            w += x;
            x = 0;
        }
        if((x + w) > WIDTH) {    // Clip right
            w = (WIDTH - x);
        }
        if(w > 0) {    // Proceed only if width is positive
            uint8_t *pBuf = &buffer[(y / 8) * WIDTH + x], mask = 1 << (y & 7);
            switch(color) {
                case MAX3000_LIGHT:
                    while(w--) {
                        *pBuf++ |= mask;
                    };
                    break;
                case MAX3000_DARK:
                    mask = ~mask;
                    while(w--) {
                        *pBuf++ &= mask;
                    };
                    break;
                case MAX3000_INVERSE:
                    while(w--) {
                        *pBuf++ ^= mask;
                    };
                    break;
            }
        }
    }
}

void MAX3000_GFX::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
    bool bSwap = false;
    switch(rotation) {
        case 1:
            // 90 degree rotation, swap x & y for rotation,
            // then invert x and adjust x for h (now to become w)
            bSwap = true;
            MAX3000_swap(x, y);
            x = WIDTH - x - 1;
            x -= (h - 1);
            break;
        case 2:
            // 180 degree rotation, invert x and y, then shift y around for height.
            x = WIDTH - x - 1;
            y = HEIGHT - y - 1;
            y -= (h - 1);
            break;
        case 3:
            // 270 degree rotation, swap x & y for rotation, then invert y
            bSwap = true;
            MAX3000_swap(x, y);
            y = HEIGHT - y - 1;
            break;
    }

    if(bSwap)
        drawFastHLineInternal(x, y, h, color);
    else
        drawFastVLineInternal(x, y, h, color);
}

void MAX3000_GFX::drawFastVLineInternal(int16_t x, int16_t __y, int16_t __h, uint16_t color) {
    if((x >= 0) && (x < WIDTH)) {    // X coord in bounds?
        if(__y < 0) {                // Clip top
            __h += __y;
            __y = 0;
        }
        if((__y + __h) > HEIGHT) {    // Clip bottom
            __h = (HEIGHT - __y);
        }
        if(__h > 0) {    // Proceed only if height is now positive
            // this display doesn't need ints for coordinates,
            // use local byte registers for faster juggling
            uint8_t y = __y, h = __h;
            uint8_t * pBuf = &buffer[(y / 8) * WIDTH + x];

            // do the first partial byte, if necessary - this requires some masking
            uint8_t mod = (y & 7);
            if(mod) {
                // mask off the high n bits we want to set
                mod = 8 - mod;
                // note - lookup table results in a nearly 10% performance
                // improvement in fill* functions
                // uint8_t mask = ~(0xFF >> mod);
                static const uint8_t PROGMEM premask[8] = { 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE };
                uint8_t mask                            = pgm_read_byte(&premask[mod]);
                // adjust the mask if we're not going to reach the end of this byte
                if(h < mod)
                    mask &= (0XFF >> (mod - h));

                switch(color) {
                    case MAX3000_LIGHT:
                        *pBuf |= mask;
                        break;
                    case MAX3000_DARK:
                        *pBuf &= ~mask;
                        break;
                    case MAX3000_INVERSE:
                        *pBuf ^= mask;
                        break;
                }
                pBuf += WIDTH;
            }

            if(h >= mod) {    // More to go?
                h -= mod;
                // Write solid bytes while we can - effectively 8 rows at a time
                if(h >= 8) {
                    if(color == MAX3000_INVERSE) {
                        // separate copy of the code so we don't impact performance of
                        // black/white write version with an extra comparison per loop
                        do {
                            *pBuf ^= 0xFF;    // Invert byte
                            pBuf += WIDTH;    // Advance pointer 8 rows
                            h -= 8;           // Subtract 8 rows from height
                        } while(h >= 8);
                    } else {
                        // store a local value to work with
                        uint8_t val = (color != MAX3000_DARK) ? 255 : 0;
                        do {
                            *pBuf = val;      // Set byte
                            pBuf += WIDTH;    // Advance pointer 8 rows
                            h -= 8;           // Subtract 8 rows from height
                        } while(h >= 8);
                    }
                }

                if(h) {    // Do the final partial byte, if necessary
                    mod = h & 7;
                    // this time we want to mask the low bits of the byte,
                    // vs the high bits we did above
                    // uint8_t mask = (1 << mod) - 1;
                    // note - lookup table results in a nearly 10% performance
                    // improvement in fill* functions
                    static const uint8_t PROGMEM postmask[8] = { 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F };
                    uint8_t mask                             = pgm_read_byte(&postmask[mod]);
                    switch(color) {
                        case MAX3000_LIGHT:
                            *pBuf |= mask;
                            break;
                        case MAX3000_DARK:
                            *pBuf &= ~mask;
                            break;
                        case MAX3000_INVERSE:
                            *pBuf ^= mask;
                            break;
                    }
                }
            }
        }    // endif positive height
    }        // endif x in bounds
}
