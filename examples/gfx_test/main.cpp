/**************************************************************************
 This example is for a single MAX3000 display.
 Modified from Adafruit_SDD1306 example.

 Original License Text:
 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!
 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 **************************************************************************/

#include <Arduino.h>
#include <MAX3000_Font6.h>
#include <MAX3000_GFX.h>

#define NUMFLAKES 8    // Number of stars in the animation example

#define DISPLAY_HEIGHT 16
#define DISPLAY_WIDTH 28

#define LED_BRIGHTNESS 64    // Brightness from 0 to 255

#define LOGO_HEIGHT 16
#define LOGO_WIDTH 16
static const unsigned char PROGMEM logo_bmp[] = {
    0b00000000, 0b11000000, 0b00000001, 0b11000000, 0b00000001, 0b11000000,
    0b00000011, 0b11100000, 0b11110011, 0b11100000, 0b11111110, 0b11111000,
    0b01111110, 0b11111111, 0b00110011, 0b10011111, 0b00011111, 0b11111100,
    0b00001101, 0b01110000, 0b00011011, 0b10100000, 0b00111111, 0b11100000,
    0b00111111, 0b11110000, 0b01111100, 0b11110000, 0b01110000, 0b01110000,
    0b00000000, 0b00110000
};

#if defined(ARDUINO_ARCH_STM32)
#define MAX_MOSI_PIN PA7     // Pin connected to MTX_DIN
#define MAX_SCLK_PIN PA5     // Pin connected to MTX_CLK
#define MAX_LAT_PIN PB10     // Pin connected to MTX_LAT
#define MAX_RST_PIN PB4      // Pin connected to MTX_RST
#define MAX_PULSE_PIN PB5    // Pin connected to PULSE_ENABLE
#define MAX_COL_PIN PB3      // Pin connected to COL_ENABLE_N
#define MAX_ROW_PIN PA10     // Pin connected to ROW_ENABLE_N
#define MAX_PWM_PIN PC7      // Pin connected to LED_ILLUM
#elif defined(ESP32)
#define MAX_MOSI_PIN 23     // Pin connected to MTX_DIN
#define MAX_SCLK_PIN 18     // Pin connected to MTX_CLK
#define MAX_LAT_PIN 16      // Pin connected to MTX_LAT
#define MAX_RST_PIN 17      // Pin connected to MTX_RST
#define MAX_PULSE_PIN 25    // Pin connected to PULSE_ENABLE
#define MAX_COL_PIN 26      // Pin connected to COL_ENABLE_N
#define MAX_ROW_PIN 27      // Pin connected to ROW_ENABLE_N
#define MAX_PWM_PIN 22      // Pin connected to LED_ILLUM
#else
#define MAX_MOSI_PIN 11    // Pin connected to MTX_DIN
#define MAX_SCLK_PIN 13    // Pin connected to MTX_CLK
#define MAX_LAT_PIN 6      // Pin connected to MTX_LAT
#define MAX_RST_PIN 5      // Pin connected to MTX_RST
#define MAX_PULSE_PIN 4    // Pin connected to PULSE_ENABLE
#define MAX_COL_PIN 3      // Pin connected to COL_ENABLE_N
#define MAX_ROW_PIN 2      // Pin connected to ROW_ENABLE_N
#define MAX_PWM_PIN 9      // Pin connected to LED_ILLUM
#endif

MAX3000_GFX display(MAX3000_Config(DISPLAY_WIDTH, DISPLAY_HEIGHT, MAX_MOSI_PIN,
    MAX_SCLK_PIN, MAX_LAT_PIN, MAX_RST_PIN,
    MAX_PULSE_PIN, MAX_COL_PIN, MAX_ROW_PIN));

void testdrawline() {
    int16_t i;

    display.clearDisplay();    // Clear display buffer

    for(i = 0; i < display.width(); i += 4) {
        display.drawLine(0, 0, i, display.height() - 1, MAX3000_LIGHT);
        display.display();    // Update screen with each newly-drawn line
        delay(1);
    }
    for(i = 0; i < display.height(); i += 4) {
        display.drawLine(0, 0, display.width() - 1, i, MAX3000_LIGHT);
        display.display();
        delay(1);
    }
    delay(250);

    display.clearDisplay();

    for(i = 0; i < display.width(); i += 4) {
        display.drawLine(0, display.height() - 1, i, 0, MAX3000_LIGHT);
        display.display();
        delay(1);
    }
    for(i = display.height() - 1; i >= 0; i -= 4) {
        display.drawLine(0, display.height() - 1, display.width() - 1, i,
            MAX3000_LIGHT);
        display.display();
        delay(1);
    }
    delay(250);

    display.clearDisplay();

    for(i = display.width() - 1; i >= 0; i -= 4) {
        display.drawLine(display.width() - 1, display.height() - 1, i, 0,
            MAX3000_LIGHT);
        display.display();
        delay(1);
    }
    for(i = display.height() - 1; i >= 0; i -= 4) {
        display.drawLine(display.width() - 1, display.height() - 1, 0, i,
            MAX3000_LIGHT);
        display.display();
        delay(1);
    }
    delay(250);

    display.clearDisplay();

    for(i = 0; i < display.height(); i += 4) {
        display.drawLine(display.width() - 1, 0, 0, i, MAX3000_LIGHT);
        display.display();
        delay(1);
    }
    for(i = 0; i < display.width(); i += 4) {
        display.drawLine(display.width() - 1, 0, i, display.height() - 1,
            MAX3000_LIGHT);
        display.display();
        delay(1);
    }

    delay(2000);    // Pause for 2 seconds
}

void testdrawrect(void) {
    display.clearDisplay();

    for(int16_t i = 0; i < min(display.width(), display.height()) / 2; i += 2) {
        display.drawRect(i, i, display.width() - 2 * i, display.height() - 2 * i,
            MAX3000_LIGHT);
        display.display();    // Update screen with each newly-drawn rectangle
        delay(1);
    }

    delay(2000);
}

void testfillrect(void) {
    display.clearDisplay();

    for(int16_t i = 0; i < display.height() / 2; i += 3) {
        // The INVERSE color is used so rectangles alternate white/black
        display.fillRect(i, i, display.width() - i * 2, display.height() - i * 2,
            MAX3000_INVERSE);
        display.display();    // Update screen with each newly-drawn rectangle
        delay(1);
    }

    delay(2000);
}

void testdrawcircle(void) {
    display.clearDisplay();

    for(int16_t i = 0; i < max(display.width(), display.height()) / 2; i += 2) {
        display.drawCircle(display.width() / 2, display.height() / 2, i,
            MAX3000_LIGHT);
        display.display();
        delay(1);
    }

    delay(2000);
}

void testfillcircle(void) {
    display.clearDisplay();

    for(int16_t i = max(display.width(), display.height()) / 2; i > 0; i -= 3) {
        // The INVERSE color is used so circles alternate white/black
        display.fillCircle(display.width() / 2, display.height() / 2, i,
            MAX3000_INVERSE);
        display.display();    // Update screen with each newly-drawn circle
        delay(1);
    }

    delay(2000);
}

void testdrawroundrect(void) {
    display.clearDisplay();

    for(int16_t i = 0; i < min(display.width(), display.height()) / 2 - 2; i += 2) {
        display.drawRoundRect(i, i, display.width() - 2 * i,
            display.height() - 2 * i, display.height() / 4,
            MAX3000_LIGHT);
        display.display();
        delay(1);
    }

    delay(2000);
}

void testfillroundrect(void) {
    display.clearDisplay();

    for(int16_t i = 0; i < display.height() / 2 - 2; i += 2) {
        // The INVERSE color is used so round-rects alternate white/black
        display.fillRoundRect(i, i, display.width() - 2 * i,
            display.height() - 2 * i, display.height() / 4,
            MAX3000_INVERSE);
        display.display();
        delay(1);
    }

    delay(2000);
}

void testdrawtriangle(void) {
    display.clearDisplay();

    for(int16_t i = 0; i < max(display.width(), display.height()) / 2; i += 5) {
        display.drawTriangle(display.width() / 2, display.height() / 2 - i,
            display.width() / 2 - i, display.height() / 2 + i,
            display.width() / 2 + i, display.height() / 2 + i,
            MAX3000_LIGHT);
        display.display();
        delay(1);
    }

    delay(2000);
}

void testfilltriangle(void) {
    display.clearDisplay();

    for(int16_t i = max(display.width(), display.height()) / 2; i > 0; i -= 5) {
        // The INVERSE color is used so triangles alternate white/black
        display.fillTriangle(display.width() / 2, display.height() / 2 - i,
            display.width() / 2 - i, display.height() / 2 + i,
            display.width() / 2 + i, display.height() / 2 + i,
            MAX3000_INVERSE);
        display.display();
        delay(1);
    }

    delay(2000);
}

void testdrawchar(void) {
    display.clearDisplay();

    display.setTextSize(1);                 // Normal 1:1 pixel scale
    display.setTextColor(MAX3000_LIGHT);    // Draw white text
    display.setCursor(0, 0);                // Start at top-left corner
    display.cp437(true);                    // Use full 256 char 'Code Page 437' font

    // Not all the characters will fit on the display. This is normal.
    // Library will draw what it can and the rest will be clipped.
    for(int16_t i = 0x41; i < 256; i++) {
        if(i == '\n')
            display.write(' ');
        else
            display.write(i);
    }

    display.display();
    delay(2000);
}

void testdrawstyles(void) {
    display.clearDisplay();

    display.setFont(&luminator_6);
    display.setTextSize(1);                 // Normal 1:1 pixel scale
    display.setTextColor(MAX3000_LIGHT);    // Draw white text
    display.setCursor(0, 7);                // Start at top-left corner
    display.setTextWrap(false);
    display.println(F("Test"));
    display.setCursor(0, 15);    // Start at top-left corner
    display.println(F("1, 2, 3"));

    display.display();
    delay(2000);
}

void testdrawbitmap(void) {
    display.clearDisplay();

    display.drawBitmap((display.width() - LOGO_WIDTH) / 2,
        (display.height() - LOGO_HEIGHT) / 2, logo_bmp, LOGO_WIDTH,
        LOGO_HEIGHT, 1);
    display.display();
    delay(1000);
}

#define XPOS 0    // Indexes into the 'icons' array in function below
#define YPOS 1
#define DELTAY 2

void testanimate(const uint8_t * bitmap, uint8_t w, uint8_t h) {
    int8_t f, icons[NUMFLAKES][3];

    // Initialize 'star' positions
    for(f = 0; f < NUMFLAKES; f++) {
        icons[f][XPOS]   = random(1 - LOGO_WIDTH, display.width());
        icons[f][YPOS]   = -LOGO_HEIGHT;
        icons[f][DELTAY] = random(1, 6);
        Serial.print(F("x: "));
        Serial.print(icons[f][XPOS], DEC);
        Serial.print(F(" y: "));
        Serial.print(icons[f][YPOS], DEC);
        Serial.print(F(" dy: "));
        Serial.println(icons[f][DELTAY], DEC);
    }

    display.setFont(&luminator_6);

    for(;;) {                      // Loop forever...
        display.clearDisplay();    // Clear the display buffer

        // Draw each star:
        for(f = 0; f < NUMFLAKES; f++) {
            display.setCursor(icons[f][XPOS], icons[f][YPOS]);
            display.print('*');
            // display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h,
            // MAX3000_LIGHT);
        }

        display.display();    // Show the display buffer on the screen
        delay(200);           // Pause for 1/10 second

        // Then update coordinates of each flake...
        for(f = 0; f < NUMFLAKES; f++) {
            icons[f][YPOS] += icons[f][DELTAY];
            // If star is off the bottom of the screen...
            if(icons[f][YPOS] >= display.height() + 8) {
                // Reinitialize to a random position, just off the top
                icons[f][XPOS]   = random(1 - LOGO_WIDTH, display.width());
                icons[f][YPOS]   = -LOGO_HEIGHT;
                icons[f][DELTAY] = random(1, 6);
            }
        }
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("Display Begin");

    pinMode(MAX_PWM_PIN, OUTPUT);
    analogWrite(MAX_PWM_PIN, LED_BRIGHTNESS);

    display.begin();
    display.setDissolveEnable(true);

    // Turn on User LED on driver
    display.setUserLED(0, true);

    display.clearDisplay();
    display.printDisplay();
    display.display();

    // Draw a single pixel in white
    delay(2000);    // Pause for 2 seconds
    display.drawPixel(10, 10, MAX3000_LIGHT);

    display.printDisplay();
    display.display();
    delay(2000);    // Pause for 2 seconds

    display.clearDisplay();
    display.printDisplay();
    display.display();

    // Turn off User LED on driver
    display.setUserLED(0, false);

    // display.display() is NOT necessary after every single drawing command,
    // unless that's what you want...rather, you can batch up a bunch of
    // drawing operations and then update the screen all at once by calling
    // display.display(). These examples demonstrate both approaches...

    testdrawline();    // Draw many lines

    testdrawrect();    // Draw rectangles (outlines)

    testfillrect();    // Draw rectangles (filled)

    testdrawcircle();    // Draw circles (outlines)

    testfillcircle();    // Draw circles (filled)

    testdrawroundrect();    // Draw rounded rectangles (outlines)

    testfillroundrect();    // Draw rounded rectangles (filled)

    testdrawtriangle();    // Draw triangles (outlines)

    testfilltriangle();    // Draw triangles (filled)

    testdrawchar();    // Draw characters of the default font

    display.setRotation(1);
    testdrawchar();    // Draw characters of the default font, rotated
    display.setRotation(0);

    testdrawstyles();    // Draw 'stylized' characters

    testdrawbitmap();    // Draw a small bitmap image

    // Invert and restore display, pausing in-between
    display.invertDisplay(true);
    delay(1000);
    display.invertDisplay(false);
    delay(1000);

    testanimate(logo_bmp, LOGO_WIDTH, LOGO_HEIGHT);    // Animate bitmaps
}

void loop() {
}