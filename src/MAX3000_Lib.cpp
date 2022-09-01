/**
 * @file MAX3000_Lib.cpp
 *
 * Library used to control the output of a Luminator MAX3000 Display.
 *
 * For use with https://github.com/NietoSkunk/FlippyDriver MAX3000 Driver.
 *
 * Uses either hardware or software SPI to load data into the shift registers
 * of the driver, as well as I2C to set PWM and read temperature.
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

#include <Arduino.h>
#include <MAX3000_Lib.h>

#ifdef __AVR__
#include <avr/pgmspace.h>
#elif defined(ESP8266) || defined(ESP32) || defined(ARDUINO_ARCH_RP2040)
#include <pgmspace.h>
#else
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))    ///< PROGMEM workaround for non-AVR
#endif

#if !defined(__ARM_ARCH) && !defined(ENERGIA) && !defined(ESP8266) && !defined(ESP32) && !defined(__arc__)
#include <util/delay.h>
#endif

// Extra delay when bitbanging on ESP32, which updates much faster than AVR
#if defined(ESP32)
#define BITBANG_DELAY delayMicroseconds(5);
#else
#define BITBANG_DELAY
#endif

#define MAX3000_swap(a, b) (((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b)))
#define LOAD_SR(_b, _p, _e)     \
    shiftReg[_b] &= ~(1 << _p); \
    shiftReg[_b] |= ((_e ? HIGH : LOW) << _p);
#define BUFFER_SIZE config.width *((config.height + 7) / 8)

#ifdef HAVE_PORTREG
#define MAX3000_LATCH *latPort |= latPinMask;           ///< Shift Register Latch
#define MAX3000_UNLATCH *latPort &= ~latPinMask;        ///< Shift Register Unlatch
#define MAX3000_PULSE *pulsePort |= pulsePinMask;       ///< PULSE_ENABLE active
#define MAX3000_UNPULSE *pulsePort &= ~pulsePinMask;    ///< PULSE_ENABLE inactive
#define MAX3000_PULSE_ROW *rowPort &= ~rowPinMask;      ///< ROW_ENABLE_N active
#define MAX3000_UNPULSE_ROW *rowPort |= rowPinMask;     ///< ROW_ENABLE_N inactive
#define MAX3000_PULSE_COL *colPort &= ~colPinMask;      ///< COL_ENABLE_N active
#define MAX3000_UNPULSE_COL *colPort |= colPinMask;     ///< COL_ENABLE_N inactive
#else
#define MAX3000_LATCH digitalWrite(config.lat_pin, HIGH);          ///< Shift Register Latch
#define MAX3000_UNLATCH digitalWrite(config.lat_pin, LOW);         ///< Shift Register Unlatch
#define MAX3000_PULSE digitalWrite(config.pulse_pin, HIGH);        ///< PULSE_ENABLE active
#define MAX3000_UNPULSE digitalWrite(config.pulse_pin, LOW);       ///< PULSE_ENABLE inactive
#define MAX3000_PULSE_ROW digitalWrite(config.row_pin, LOW);       ///< ROW_ENABLE_N active
#define MAX3000_UNPULSE_ROW digitalWrite(config.row_pin, HIGH);    ///< ROW_ENABLE_N inactive
#define MAX3000_PULSE_COL digitalWrite(config.col_pin, LOW);       ///< COL_ENABLE_N active
#define MAX3000_UNPULSE_COL digitalWrite(config.col_pin, HIGH);    ///< COL_ENABLE_N inactive
#endif

// SPI transactions  not present in older Arduino SPI lib
#if defined(SPI_HAS_TRANSACTION)
#define SPI_TRANSACTION_START \
    if(config.spi) config.spi->beginTransaction(spiSettings);    ///< Pre-SPI
#define SPI_TRANSACTION_END \
    if(config.spi) config.spi->endTransaction();    ///< Post-SPI
#else
#define SPI_TRANSACTION_START    ///< Dummy stand-in define
#define SPI_TRANSACTION_END      ///< keeps compiler happy
#endif

// Shift Register bit definitions on each driver
#define SR_PIN_COL_A2 0
#define SR_PIN_COL_A1 1
#define SR_PIN_COL_A0 2
#define SR_PIN_ROW_A0 3
#define SR_PIN_ROW_A1 4
#define SR_PIN_ROW_A2 5
#define SR_PIN_ROW_BANK 6
#define SR_PIN_COL_BANK0 7
#define SR_PIN_COL_SOURCE 8
#define SR_PIN_ROW_SOURCE 10
#define SR_PIN_COL_BANK1 11
#define SR_PIN_USER_LED 13

MAX3000_Lib::MAX3000_Lib(const MAX3000_Config & config_)
    : config(config_), buffer(NULL) {
    _width          = config.width;
    _height         = config.height;
    invertEnabled   = false;
    dissolveEnabled = false;
    firstUpdate     = true;

    // 250uS has been determined to be a decent compromise between frame rate and flip reliability
    pulseDuration = 250;

#ifdef SPI_HAS_TRANSACTION
    spiSettings = SPISettings(config.spi_bitrate, MSBFIRST, SPI_MODE0);
#endif
}

MAX3000_Lib::~MAX3000_Lib(void) {
    if(buffer) {
        free(buffer);
        buffer = NULL;
    }
    if(oldBuffer) {
        free(oldBuffer);
        oldBuffer = NULL;
    }
    if(shuffledIndex) {
        delete[] shuffledIndex;
        shuffledIndex = NULL;
    }
    if(shiftReg) {
        delete[] shiftReg;
        shiftReg = NULL;
    }
}

inline void
MAX3000_Lib::shiftRegWrite() {
    // Push each board through the chain, starting with the last board
    SPI_TRANSACTION_START
    for(size_t board = config.numHBoards * config.numVBoards; board > 0; --board) {
        if(config.spi) {
            (void)config.spi->transfer16(shiftReg[board - 1]);
        } else {
            for(uint16_t bit = 0x8000; bit; bit >>= 1) {
#ifdef HAVE_PORTREG
                if(shiftReg[board - 1] & bit)
                    *mosiPort |= mosiPinMask;
                else
                    *mosiPort &= ~mosiPinMask;
                BITBANG_DELAY
                *clkPort |= clkPinMask;
                BITBANG_DELAY
                *clkPort &= ~clkPinMask;
                BITBANG_DELAY
#else
                digitalWrite(config.mosi_pin, (bool)(shiftReg[board - 1] & bit));
                BITBANG_DELAY
                digitalWrite(config.sclk_pin, HIGH);
                BITBANG_DELAY
                digitalWrite(config.sclk_pin, LOW);
                BITBANG_DELAY
#endif
            }
        }
    }
    SPI_TRANSACTION_END

    // Once shift register buffer has been shifted in, latch the output pins.
    MAX3000_LATCH
    MAX3000_UNLATCH
}

bool MAX3000_Lib::begin(bool reset, bool periphBegin) {
    // Create pixel buffers
    if((!buffer) && !(buffer = (uint8_t *)malloc(BUFFER_SIZE))) {
        return false;
    }
    if((!oldBuffer) && !(oldBuffer = (uint8_t *)malloc(BUFFER_SIZE))) {
        return false;
    }
    memset(buffer, 0, BUFFER_SIZE);
    memset(oldBuffer, 0, BUFFER_SIZE);

    // Create initial index buffer, which will get shuffled on the first load.
    // Each panel received the same shuffled index for space concerns.
    if((!shuffledIndex) && !(shuffledIndex = new int[PANEL_HEIGHT * PANEL_WIDTH])) {
        return false;
    }
    for(int i = 0; i < PANEL_HEIGHT * PANEL_WIDTH; i++) {
        shuffledIndex[i] = i;
    }

    // Create buffer for output to each shift register
    if((!shiftReg) && !(shiftReg = new uint16_t[config.numVBoards * config.numHBoards])) {
        return false;
    }
    memset(shiftReg, 0, config.numVBoards * config.numHBoards * sizeof(uint16_t));

    // Set up hardware pin modes
    pinMode(config.lat_pin, OUTPUT);
    pinMode(config.rst_pin, OUTPUT);
    pinMode(config.pulse_pin, OUTPUT);
    pinMode(config.col_pin, OUTPUT);
    pinMode(config.row_pin, OUTPUT);

#ifdef HAVE_PORTREG
    latPort      = (PortReg *)portOutputRegister(digitalPinToPort(config.lat_pin));
    latPinMask   = digitalPinToBitMask(config.lat_pin);
    pulsePort    = (PortReg *)portOutputRegister(digitalPinToPort(config.pulse_pin));
    pulsePinMask = digitalPinToBitMask(config.pulse_pin);
    rowPort      = (PortReg *)portOutputRegister(digitalPinToPort(config.row_pin));
    rowPinMask   = digitalPinToBitMask(config.row_pin);
    colPort      = (PortReg *)portOutputRegister(digitalPinToPort(config.col_pin));
    colPinMask   = digitalPinToBitMask(config.col_pin);
#endif

    // Initialize SPI (either hardware or software)
    if(config.spi) {
        if(periphBegin) {
            config.spi->begin();
        }
    } else {
        pinMode(config.mosi_pin, OUTPUT);
        pinMode(config.sclk_pin, OUTPUT);
#ifdef HAVE_PORTREG
        mosiPort    = (PortReg *)portOutputRegister(digitalPinToPort(config.mosi_pin));
        mosiPinMask = digitalPinToBitMask(config.mosi_pin);
        clkPort     = (PortReg *)portOutputRegister(digitalPinToPort(config.sclk_pin));
        clkPinMask  = digitalPinToBitMask(config.sclk_pin);
        *clkPort &= ~clkPinMask;
#else
        digitalWrite(config.sclk_pin, LOW);
#endif
    }

    // Set initial non-pulse state
    MAX3000_UNPULSE
    MAX3000_UNPULSE_ROW
    MAX3000_UNPULSE_COL

    // Reset MAX3000 if requested and reset pin specified in constructor
    if(reset && (config.rst_pin >= 0)) {
        digitalWrite(config.rst_pin, HIGH);
        delay(1);
        digitalWrite(config.rst_pin, LOW);
        delay(10);
        digitalWrite(config.rst_pin, HIGH);
        delay(5);
    }

    return true;
}

void MAX3000_Lib::drawPixel(int16_t x, int16_t y, uint16_t color) {
    if((x >= 0) && (x < getWidth()) && (y >= 0) && (y < getHeight())) {
        // Pixel is in-bounds. Rotate coordinates if needed.
        switch(getDisplayRotation()) {
            case 1:
                MAX3000_swap(x, y);
                x = config.width - x - 1;
                break;
            case 2:
                x = config.width - x - 1;
                y = config.height - y - 1;
                break;
            case 3:
                MAX3000_swap(x, y);
                y = config.height - y - 1;
                break;
        }

        switch(color) {
            case MAX3000_LIGHT:
                buffer[x + (y / 8) * config.width] |= (1 << (y & 7));
                break;
            case MAX3000_DARK:
                buffer[x + (y / 8) * config.width] &= ~(1 << (y & 7));
                break;
            case MAX3000_INVERSE:
                buffer[x + (y / 8) * config.width] ^= (1 << (y & 7));
                break;
        }
    }
}

void MAX3000_Lib::clearDisplay(void) {
    memset(buffer, 0, BUFFER_SIZE);
}

bool MAX3000_Lib::getPixel(int16_t x, int16_t y) {
    if((x >= 0) && (x < getWidth()) && (y >= 0) && (y < getHeight())) {
        // Pixel is in-bounds. Rotate coordinates if needed.
        switch(getDisplayRotation()) {
            case 1:
                MAX3000_swap(x, y);
                x = config.width - x - 1;
                break;
            case 2:
                x = config.width - x - 1;
                y = config.height - y - 1;
                break;
            case 3:
                MAX3000_swap(x, y);
                y = config.height - y - 1;
                break;
        }
        return (buffer[x + (y / 8) * config.width] & (1 << (y & 7)));
    }
    return false;    // Pixel out of bounds
}

void MAX3000_Lib::setUserLED(size_t board, bool state) {
    LOAD_SR(board, SR_PIN_USER_LED, state);
    shiftRegWrite();
}

uint8_t * MAX3000_Lib::getBuffer(void) {
    return buffer;
}

void MAX3000_Lib::display(bool force) {
    if(dissolveEnabled) {
        // When dissolve mode is enabled, we want to update in a random order.
        // To maintain the random appearance, we'll shuffle on each update.
        shuffleIndex();
    }

    for(int i = 0; i < PANEL_HEIGHT * PANEL_WIDTH; ++i) {
#if defined(ESP8266)
        if(i == 0) {
            yield();
        }
#endif

        // If dissolving, pick the shuffled index
        int index = (dissolveEnabled) ? shuffledIndex[i] : i;

        size_t col = (index / PANEL_HEIGHT);
        size_t row = (index % PANEL_HEIGHT);

        bool pixelsToSet[config.numHBoards * config.numVBoards];
        bool pixelsToClear[config.numHBoards * config.numVBoards];
        memset(pixelsToSet, 0, config.numHBoards * config.numVBoards * sizeof(bool));
        memset(pixelsToClear, 0, config.numHBoards * config.numVBoards * sizeof(bool));

        // First pass: Get the set/clear
        // Since all boards share the same pulse lines, we have to figure out the
        // required operation for every board first.
        // Thus we just store what we want to do in an array, and then actually load
        // and pulse once we've checked every board.
        for(size_t board = 0; board < config.numHBoards * config.numVBoards; ++board) {
            size_t boardCol     = board % config.numHBoards;
            size_t bufferOffset = (col + boardCol * PANEL_WIDTH) + ((row / 8) * config.numHBoards * PANEL_WIDTH);

            bool newPixVal = buffer[bufferOffset] & (1 << (row & 7));
            bool oldPixVal = oldBuffer[bufferOffset] & (1 << (row & 7));

            // Check if we can skip the update.
            // If it's the first update, we need to refresh everything.
            if(!force && !firstUpdate && newPixVal == oldPixVal) {
                continue;
            }

            // Pre-select the decoder inputs for each board now.
            selectRowColumn(board, row, col);

            bool setPixel        = (newPixVal != invertEnabled);
            pixelsToSet[board]   = setPixel;
            pixelsToClear[board] = !setPixel;
        }

        // Second Pass: Turn on pixels that need to be set
        // If no change is necessary for a board, neither row or column will
        // be sourced and the pixel will remain in its existing state
        bool setChanged = false;
        for(size_t board = 0; board < config.numHBoards * config.numVBoards; ++board) {
            // Setting -> Row Set Source, Column sink
            LOAD_SR(board, SR_PIN_COL_SOURCE, LOW);
            LOAD_SR(board, SR_PIN_ROW_SOURCE, pixelsToSet[board]);
            if(pixelsToSet[board]) setChanged = true;
        }
        if(setChanged) {
            shiftRegWrite();
            setPixel();
        }

        // Second Pass: Turn off pixels that need to be cleared
        // If no change is necessary for a board, neither row or column will
        // be sourced and the pixel will remain in its existing state
        bool resetChanged = false;
        for(size_t board = 0; board < config.numHBoards * config.numVBoards; ++board) {
            // Setting -> Row Set Source, Column sink
            LOAD_SR(board, SR_PIN_COL_SOURCE, pixelsToClear[board]);
            LOAD_SR(board, SR_PIN_ROW_SOURCE, LOW);
            if(pixelsToClear[board]) resetChanged = true;
        }
        if(resetChanged) {
            shiftRegWrite();
            clearPixel();
        }
    }

    // Store current buffer to avoid unnecessary changes on next update.
    memcpy(oldBuffer, buffer, BUFFER_SIZE);
    firstUpdate = false;

#if defined(ESP8266)
    yield();
#endif
}

void MAX3000_Lib::printDisplay(Stream & stream) {
    // Print each row
    for(size_t index = 0; index < config.width * config.height; ++index) {
        size_t col = index % config.width;
        size_t row = index / config.width;

        // If first column in row and first row in a board, print header
        if(col == 0 && row % PANEL_HEIGHT == 0) {
            for(size_t b = 0; b < config.numHBoards; ++b) {
                stream.print('+');
                for(size_t i = 0; i < (2 * config.width / config.numHBoards); ++i) {
                    stream.print('-');
                }
                stream.print('+');
            }
            stream.println();
        }

        // If first column in board, print opening border
        if(col % PANEL_WIDTH == 0) {
            stream.print('|');
        }

        size_t bufferOffset = col + (row / 8) * config.width;

        uint8_t bufferVal = buffer[bufferOffset];
        bool pixVal       = bufferVal & (1 << (row & 7));

        if(pixVal) {
            invertEnabled ? stream.print("  ") : stream.print("()");
        } else {
            invertEnabled ? stream.print("()") : stream.print("  ");
        }

        // If last column in board (or last column total), print closing border
        if((col + 1) % PANEL_WIDTH == 0 || (uint8_t)col == (config.width - 1)) {
            stream.print('|');
        }

        // If the last column total, print a newline
        if((uint8_t)col == config.width - 1) {
            stream.println();

            // If last row in board (or last row total), print footer
            if((row + 1) % PANEL_HEIGHT == 0 || (uint8_t)row == (config.height - 1)) {
                for(size_t b = 0; b < config.numHBoards; ++b) {
                    stream.print('+');
                    for(size_t i = 0; i < (2 * config.width / config.numHBoards); ++i) {
                        stream.print('-');
                    }
                    stream.print('+');
                }
                stream.println();
            }

#if defined(ESP8266)
            // Yield once per row
            yield();
#endif
        }
    }

    stream.println();
}

void MAX3000_Lib::invertDisplay(bool i) {
    invertEnabled = i;

    // Send an immediate update. We didn't change the actual buffer so we have to force an update.
    display(true);
}

void MAX3000_Lib::setDisplayRotation(uint8_t x) {
    _rotation = (x & 3);
    switch(_rotation) {
        case 0:
        case 2:
            _width  = config.width;
            _height = config.height;
            break;
        case 1:
        case 3:
            _width  = config.height;
            _height = config.width;
            break;
    }
}

void MAX3000_Lib::setDissolveEnable(uint8_t param) {
    dissolveEnabled = (bool)param;
}

void MAX3000_Lib::setPulseDurationUs(uint16_t param) {
    pulseDuration = param;
}

void MAX3000_Lib::selectRowColumn(size_t board, size_t row, size_t column) {    // TODO Board Order
    // Map sequential rows and columns to the hardware pins
    const uint8_t colToCode[] = { 1, 0, 3, 2, 5, 4, 7, 6, 9, 8, 11, 10, 13, 12,
        15, 14, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27 };
    const uint8_t rowToCode[] = { 14, 1, 15, 0, 12, 3, 13, 2, 10, 5, 11, 4, 8, 7, 9, 6 };

    // Hack - Rows are reversed on the panel
    row = 15 - row;

    uint8_t rowSubCode = rowToCode[row] % 8;
    uint8_t rowBank    = rowToCode[row] / 8;

    uint8_t colSubCode = colToCode[column] % 8;
    uint8_t colBank    = colToCode[column] / 8;

    LOAD_SR(board, SR_PIN_COL_A2, colSubCode & 0x4);
    LOAD_SR(board, SR_PIN_COL_A1, colSubCode & 0x2);
    LOAD_SR(board, SR_PIN_COL_A0, colSubCode & 0x1);
    LOAD_SR(board, SR_PIN_ROW_A2, rowSubCode & 0x4);
    LOAD_SR(board, SR_PIN_ROW_A1, rowSubCode & 0x2);
    LOAD_SR(board, SR_PIN_ROW_A0, rowSubCode & 0x1);
    LOAD_SR(board, SR_PIN_ROW_BANK, rowBank & 0x1);
    LOAD_SR(board, SR_PIN_COL_BANK1, colBank & 0x2);
    LOAD_SR(board, SR_PIN_COL_BANK0, colBank & 0x1);
}

void MAX3000_Lib::setPixel() {
    // Global Pulse Enable
    MAX3000_PULSE

    // Turn on Source first, then Sink
    MAX3000_PULSE_ROW
    delayMicroseconds(5);
    MAX3000_PULSE_COL

    // Wait for Pulse Duration
    delayMicroseconds(pulseDuration);

    // Turn off Sink first, then Source
    MAX3000_UNPULSE_COL
    delayMicroseconds(5);
    MAX3000_UNPULSE_ROW

    // Global Pulse Disable
    MAX3000_UNPULSE
}

void MAX3000_Lib::clearPixel() {
    // Global Pulse Enable
    MAX3000_PULSE

    // Turn on Source first, then Sink
    MAX3000_PULSE_COL
    delayMicroseconds(5);
    MAX3000_PULSE_ROW

    // Wait for Pulse Duration
    delayMicroseconds(pulseDuration);

    // Turn off Sink first, then Source
    MAX3000_UNPULSE_ROW
    delayMicroseconds(5);
    MAX3000_UNPULSE_COL

    // Global Pulse Disable
    MAX3000_UNPULSE
}

void MAX3000_Lib::shuffleIndex() {
    for(int i = 0; i < (int)(PANEL_HEIGHT * PANEL_WIDTH); i++) {
        int n            = random(0, PANEL_HEIGHT * PANEL_WIDTH);
        int temp         = shuffledIndex[n];
        shuffledIndex[n] = shuffledIndex[i];
        shuffledIndex[i] = temp;
    }
}
