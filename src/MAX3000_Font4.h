/**
 * @file MAX3000_Font4.h
 *
 * 4-pixel tall character set.
 *
 * For use with https://github.com/NietoSkunk/FlippyDriver MAX3000 Driver.
 *
 * This library depends on https://github.com/adafruit/Adafruit-GFX-Library
 * being present on your system. Please make sure you have installed the latest
 * version before using this library.
 *
 * BSD license, all text above must be included in any redistribution.
 */

#include <Arduino.h>
#include <Adafruit_GFX.h>
// Generated with https://tchapi.github.io/Adafruit-GFX-Font-Customiser/

const uint8_t luminator_4Bitmaps[] PROGMEM = {
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00,
    0x80, 0xA0, 0xA0, 0x50, 0xF8, 0x50, 0xF8, 0x50, 0x78, 0xA0, 0x70, 0x28,
    0xF0, 0x88, 0x10, 0x20, 0x40, 0x88, 0x30, 0x40, 0xA8, 0x90, 0x68, 0x80,
    0x80, 0x40, 0x80, 0x80, 0x80, 0x40, 0x80, 0x40, 0x40, 0x40, 0x80, 0xA0,
    0x40, 0xA0, 0x40, 0xE0, 0x40, 0x40, 0x40, 0x80, 0xE0, 0xC0, 0xC0, 0x20,
    0x20, 0x40, 0x80, 0x80, 0x40, 0xA0, 0xA0, 0xA0, 0x40, 0x40, 0xC0, 0x40,
    0x40, 0xE0, 0xC0, 0x20, 0x60, 0x80, 0xE0, 0xE0, 0x20, 0x60, 0x20, 0xE0,
    0xA0, 0xA0, 0xE0, 0x20, 0x20, 0xE0, 0x80, 0xE0, 0x20, 0xC0, 0xE0, 0x80,
    0xE0, 0xA0, 0xE0, 0xE0, 0x20, 0x20, 0x40, 0x40, 0xE0, 0xA0, 0xE0, 0xA0,
    0xE0, 0xE0, 0xA0, 0xE0, 0x20, 0xC0, 0x80, 0x00, 0x80, 0x40, 0x00, 0x40,
    0x80, 0x20, 0x40, 0x80, 0x40, 0x20, 0xE0, 0x00, 0xE0, 0x80, 0x40, 0x20,
    0x40, 0x80, 0xE0, 0x20, 0x60, 0x00, 0x40, 0x60, 0x90, 0xD0, 0xA0, 0x70,
    0x40, 0xA0, 0xE0, 0xA0, 0xA0, 0xC0, 0xA0, 0xC0, 0xA0, 0xC0, 0x60, 0x80,
    0x80, 0x80, 0x60, 0xC0, 0xA0, 0xA0, 0xA0, 0xC0, 0xE0, 0x80, 0xC0, 0x80,
    0xE0, 0xE0, 0x80, 0xC0, 0x80, 0x80, 0x60, 0x80, 0x80, 0xA0, 0x60, 0xA0,
    0xA0, 0xE0, 0xA0, 0xA0, 0xE0, 0x40, 0x40, 0x40, 0xE0, 0x20, 0x20, 0x20,
    0xA0, 0x40, 0xA0, 0xC0, 0x80, 0xC0, 0xA0, 0x80, 0x80, 0x80, 0x80, 0xE0,
    0x50, 0xA8, 0xA8, 0x88, 0x88, 0x90, 0xD0, 0xB0, 0x90, 0x90, 0x60, 0x90,
    0x90, 0x90, 0x60, 0xE0, 0x90, 0xE0, 0x80, 0x80, 0x60, 0x90, 0x90, 0x90,
    0x68, 0xE0, 0x90, 0xE0, 0xA0, 0x90, 0x60, 0x80, 0x40, 0x20, 0xC0, 0xE0,
    0x40, 0x40, 0x40, 0x40, 0x90, 0x90, 0x90, 0x90, 0x60, 0xA0, 0xA0, 0xA0,
    0xA0, 0x40, 0x88, 0x88, 0xA8, 0xA8, 0x50, 0xA0, 0xA0, 0x40, 0xA0, 0xA0,
    0xA0, 0xA0, 0x40, 0x40, 0x40, 0xE0, 0x20, 0x40, 0x80, 0xE0, 0xC0, 0x80,
    0x80, 0x80, 0xC0, 0x80, 0x80, 0x40, 0x20, 0x20, 0xC0, 0x40, 0x40, 0x40,
    0xC0, 0x40, 0xA0, 0xE0, 0x80, 0x40, 0x60, 0xA0, 0xA0, 0x50, 0x80, 0x80,
    0xC0, 0xA0, 0xC0, 0x60, 0x80, 0x80, 0x60, 0x20, 0x20, 0x60, 0xA0, 0x60,
    0x60, 0x90, 0xE0, 0x80, 0x60, 0x60, 0x40, 0xE0, 0x40, 0x40, 0x60, 0x90,
    0x90, 0x70, 0x10, 0x60, 0x80, 0x80, 0xC0, 0xA0, 0xA0, 0x80, 0x00, 0x80,
    0x80, 0x80, 0x20, 0x00, 0x20, 0xA0, 0x40, 0x80, 0xA0, 0xC0, 0xA0, 0xA0,
    0x80, 0x80, 0x80, 0x80, 0x80, 0x50, 0xA8, 0x88, 0x88, 0x40, 0xA0, 0xA0,
    0xA0, 0x40, 0xA0, 0xA0, 0x40, 0x40, 0xA0, 0xA0, 0xC0, 0x80, 0x80, 0x40,
    0xA0, 0xA0, 0x60, 0x20, 0x30, 0x00, 0x60, 0x80, 0x80, 0x80, 0x60, 0x80,
    0x40, 0x20, 0xC0, 0x80, 0x80, 0xE0, 0x80, 0x60, 0xA0, 0xA0, 0xA0, 0xE0,
    0xA0, 0xA0, 0xA0, 0x40, 0x88, 0x88, 0xA8, 0x50, 0xA0, 0x40, 0xA0, 0xA0,
    0xA0, 0xE0, 0x20, 0xE0, 0xE0, 0x20, 0x40, 0x80, 0xE0
};

const GFXglyph luminator_4Glyphs[] PROGMEM = {
    { 0, 8, 0, 3, 0, -8 },      // 0x20 ' '
    { 8, 8, 5, 2, 0, -7 },      // 0x21 '!'
    { 13, 8, 2, 4, 0, -7 },     // 0x22 '"'
    { 15, 8, 5, 6, 0, -7 },     // 0x23 '#'
    { 20, 8, 5, 6, 0, -7 },     // 0x24 '$'
    { 25, 8, 5, 6, 0, -7 },     // 0x25 '%'
    { 30, 8, 5, 6, 0, -7 },     // 0x26 '&'
    { 35, 8, 2, 2, 0, -7 },     // 0x27 '''
    { 37, 8, 5, 3, 0, -7 },     // 0x28 '('
    { 42, 8, 5, 3, 0, -7 },     // 0x29 ')'
    { 47, 8, 3, 4, 0, -7 },     // 0x2A '*'
    { 50, 8, 3, 4, 0, -6 },     // 0x2B '+'
    { 53, 8, 3, 3, 0, -5 },     // 0x2C ','
    { 56, 8, 1, 4, 0, -5 },     // 0x2D '-'
    { 57, 8, 2, 3, 0, -4 },     // 0x2E '.'
    { 59, 8, 5, 4, 0, -7 },     // 0x2F '/'
    { 64, 8, 5, 4, 0, -7 },     // 0x30 '0'
    { 69, 8, 5, 4, 0, -7 },     // 0x31 '1'
    { 74, 8, 5, 4, 0, -7 },     // 0x32 '2'
    { 79, 8, 5, 4, 0, -7 },     // 0x33 '3'
    { 84, 8, 5, 4, 0, -7 },     // 0x34 '4'
    { 89, 8, 5, 4, 0, -7 },     // 0x35 '5'
    { 94, 8, 5, 4, 0, -7 },     // 0x36 '6'
    { 99, 8, 5, 4, 0, -7 },     // 0x37 '7'
    { 104, 8, 5, 4, 0, -7 },    // 0x38 '8'
    { 109, 8, 5, 4, 0, -7 },    // 0x39 '9'
    { 114, 8, 3, 2, 0, -6 },    // 0x3A ':'
    { 117, 8, 4, 3, 0, -6 },    // 0x3B ';'
    { 121, 8, 5, 4, 0, -7 },    // 0x3C '<'
    { 126, 8, 3, 4, 0, -6 },    // 0x3D '='
    { 129, 8, 5, 4, 0, -7 },    // 0x3E '>'
    { 134, 8, 5, 4, 0, -7 },    // 0x3F '?'
    { 139, 8, 5, 4, 0, -7 },    // 0x40 '@'
    { 144, 8, 5, 4, 0, -7 },    // 0x41 'A'
    { 149, 8, 5, 4, 0, -7 },    // 0x42 'B'
    { 154, 8, 5, 4, 0, -7 },    // 0x43 'C'
    { 159, 8, 5, 4, 0, -7 },    // 0x44 'D'
    { 164, 8, 5, 4, 0, -7 },    // 0x45 'E'
    { 169, 8, 5, 4, 0, -7 },    // 0x46 'F'
    { 174, 8, 5, 4, 0, -7 },    // 0x47 'G'
    { 179, 8, 5, 4, 0, -7 },    // 0x48 'H'
    { 184, 8, 5, 4, 0, -7 },    // 0x49 'I'
    { 189, 8, 5, 4, 0, -7 },    // 0x4A 'J'
    { 194, 8, 5, 4, 0, -7 },    // 0x4B 'K'
    { 199, 8, 5, 4, 0, -7 },    // 0x4C 'L'
    { 204, 8, 5, 6, 0, -7 },    // 0x4D 'M'
    { 209, 8, 5, 6, 0, -7 },    // 0x4E 'N'
    { 214, 8, 5, 5, 0, -7 },    // 0x4F 'O'
    { 219, 8, 5, 5, 0, -7 },    // 0x50 'P'
    { 224, 8, 5, 6, 0, -7 },    // 0x51 'Q'
    { 229, 8, 5, 5, 0, -7 },    // 0x52 'R'
    { 234, 8, 5, 4, 0, -7 },    // 0x53 'S'
    { 239, 8, 5, 4, 0, -7 },    // 0x54 'T'
    { 244, 8, 5, 5, 0, -7 },    // 0x55 'U'
    { 249, 8, 5, 4, 0, -7 },    // 0x56 'V'
    { 254, 8, 5, 6, 0, -7 },    // 0x57 'W'
    { 259, 8, 5, 4, 0, -7 },    // 0x58 'X'
    { 264, 8, 5, 4, 0, -7 },    // 0x59 'Y'
    { 269, 8, 5, 4, 0, -7 },    // 0x5A 'Z'
    { 274, 8, 5, 3, 0, -7 },    // 0x5B '['
    { 279, 8, 5, 4, 0, -6 },    // 0x5C '\'
    { 284, 8, 5, 3, 0, -7 },    // 0x5D ']'
    { 289, 8, 2, 4, 0, -7 },    // 0x5E '^'
    { 291, 8, 1, 4, 0, -3 },    // 0x5F '_'
    { 292, 8, 2, 3, 0, -7 },    // 0x60 '`'
    { 294, 8, 4, 5, 0, -6 },    // 0x61 'a'
    { 298, 8, 5, 4, 0, -7 },    // 0x62 'b'
    { 303, 8, 4, 4, 0, -6 },    // 0x63 'c'
    { 307, 8, 5, 4, 0, -7 },    // 0x64 'd'
    { 312, 8, 5, 5, 0, -7 },    // 0x65 'e'
    { 317, 8, 5, 4, 0, -7 },    // 0x66 'f'
    { 322, 8, 6, 5, 0, -6 },    // 0x67 'g'
    { 328, 8, 5, 4, 0, -7 },    // 0x68 'h'
    { 333, 8, 5, 2, 0, -7 },    // 0x69 'i'
    { 338, 8, 5, 4, 0, -7 },    // 0x6A 'j'
    { 343, 8, 5, 4, 0, -7 },    // 0x6B 'k'
    { 348, 8, 5, 2, 0, -7 },    // 0x6C 'l'
    { 353, 8, 4, 6, 0, -6 },    // 0x6D 'm'
    { 357, 8, 4, 5, 0, -6 },    // 0x6E 'n'
    { 361, 8, 4, 4, 0, -6 },    // 0x6F 'o'
    { 365, 8, 6, 4, 0, -6 },    // 0x70 'p'
    { 371, 8, 7, 5, 0, -6 },    // 0x71 'q'
    { 378, 8, 4, 4, 0, -6 },    // 0x72 'r'
    { 382, 8, 5, 4, 0, -7 },    // 0x73 's'
    { 387, 8, 5, 4, 0, -7 },    // 0x74 't'
    { 392, 8, 4, 4, 0, -6 },    // 0x75 'u'
    { 396, 8, 4, 4, 0, -6 },    // 0x76 'v'
    { 400, 8, 4, 6, 0, -6 },    // 0x77 'w'
    { 404, 8, 3, 4, 0, -5 },    // 0x78 'x'
    { 407, 8, 5, 5, 0, -5 },    // 0x79 'y'
    { 412, 8, 5, 4, 0, -7 }     // 0x7A 'z'
};

const GFXfont luminator_4 PROGMEM = {
    (uint8_t *)luminator_4Bitmaps, (GFXglyph *)luminator_4Glyphs, 0x20, 0x7E, 13
};