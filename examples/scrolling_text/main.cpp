#include <Arduino.h>

#include <MAX3000_Font6.h>
#include <MAX3000_GFX.h>

#define DISPLAY_HEIGHT 16
#define DISPLAY_WIDTH 28

#define LED_BRIGHTNESS 64    // Brightness from 0 to 255

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

String firstLine        = "Scrolling Test!";
uint16_t firstLineWidth = 0;
int firstOffset         = 0;

String secondLine        = "Backwards too...";
uint16_t secondLineWidth = 0;
int secondOffset         = 0;

void setup() {
    Serial.begin(115200);
    Serial.println("Display Begin");

    pinMode(MAX_PWM_PIN, OUTPUT);
    analogWrite(MAX_PWM_PIN, LED_BRIGHTNESS);

    display.begin();
    display.setDissolveEnable(true);
    display.setConstantFrameRate(true);    // Ensure constant scroll speed

    display.clearDisplay();
    display.printDisplay();
    display.display();

    display.setFont(&luminator_6);
    display.setTextSize(1);
    display.setTextWrap(false);
    display.setTextColor(MAX3000_LIGHT);

    int16_t tmp;
    uint16_t utmp;
    display.getTextBounds(firstLine, 0, 0, &tmp, &tmp, &firstLineWidth, &utmp);
    display.getTextBounds(secondLine, 0, 0, &tmp, &tmp, &secondLineWidth, &utmp);

    firstOffset  = display.width() + 1;
    secondOffset = display.width() + 1;
}

void loop() {
    display.clearDisplay();
    display.fillRect(0, display.height() / 2, display.width(), display.height() / 2, MAX3000_LIGHT);

    display.setTextColor(MAX3000_LIGHT);
    display.setCursor(firstOffset, display.height() / 2 - 1);
    display.print(firstLine);

    display.setTextColor(MAX3000_DARK);
    display.setCursor(secondOffset, display.height() - 1);
    display.print(secondLine);

    firstOffset -= 1;
    secondOffset += 2;

    if(firstOffset < -firstLineWidth) {
        firstOffset = display.width();
    }
    if(secondOffset > display.width()) {
        secondOffset = -secondLineWidth;
    }

    display.printDisplay();
    display.display();
    delay(5);
}