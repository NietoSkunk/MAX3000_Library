#include <MAX3000_Lib.h>

#define DISPLAY_HEIGHT 16
#define DISPLAY_WIDTH 28

#define LED_BRIGHTNESS 64    // Brightness from 0 to 255

#if defined(WIRINGPI)
#define MAX_MOSI_PIN 12    // Pin connected to MTX_DIN
#define MAX_SCLK_PIN 14    // Pin connected to MTX_CLK
#define MAX_LAT_PIN 0      // Pin connected to MTX_LAT
#define MAX_RST_PIN 2      // Pin connected to MTX_RST
#define MAX_PULSE_PIN 3    // Pin connected to PULSE_ENABLE
#define MAX_COL_PIN 25     // Pin connected to COL_ENABLE_N
#define MAX_ROW_PIN 21     // Pin connected to ROW_ENABLE_N
#define MAX_PWM_PIN 23     // Pin connected to LED_ILLUM
#elif defined(ARDUINO_ARCH_STM32)
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
#else                       // Uno / Mega
#define MAX_MOSI_PIN 11     // Pin connected to MTX_DIN
#define MAX_SCLK_PIN 13     // Pin connected to MTX_CLK
#define MAX_LAT_PIN 6       // Pin connected to MTX_LAT
#define MAX_RST_PIN 5       // Pin connected to MTX_RST
#define MAX_PULSE_PIN 4     // Pin connected to PULSE_ENABLE
#define MAX_COL_PIN 3       // Pin connected to COL_ENABLE_N
#define MAX_ROW_PIN 2       // Pin connected to ROW_ENABLE_N
#define MAX_PWM_PIN 9       // Pin connected to LED_ILLUM
#endif

MAX3000_Display display(MAX3000_Config(DISPLAY_WIDTH, DISPLAY_HEIGHT,
    MAX_MOSI_PIN, MAX_SCLK_PIN, MAX_LAT_PIN, MAX_RST_PIN,
    MAX_PULSE_PIN, MAX_COL_PIN, MAX_ROW_PIN));

void setup() {
    Serial.begin(115200);
    Serial.println("Display Begin");

#ifdef WIRINGPI
    pinMode(MAX_PWM_PIN, PWM_OUTPUT);
    pwmWrite(MAX_PWM_PIN, LED_BRIGHTNESS * 4);
#else
    pinMode(MAX_PWM_PIN, OUTPUT);
    analogWrite(MAX_PWM_PIN, LED_BRIGHTNESS);
#endif

    display.begin();
    display.clearDisplay();
    display.printDisplay();
    display.display();
}

int stage        = 0;
const int offset = 10;

void loop() {
    display.clearDisplay();

    for(int x = offset; x < display.width() + offset; ++x) {
        for(int y = offset; y < display.height() + offset; ++y) {
            bool on = ((x - stage) / 4) % 2 != ((y - stage) / 4) % 2;
            display.drawPixel(x - offset, y - offset, on);
        }
    }

    stage++;
    if(stage == 4) {
        stage = 0;
    }

    display.printDisplay();
    display.display();
    delay(250);
}

#ifdef WIRINGPI
int main(int argc, char ** argv) {
    wiringPiSetup();
    setup();
    while(1) {
        loop();
    }
}
#endif
