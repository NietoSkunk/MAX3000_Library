/**
 * @file MAX3000_Lib.h
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

#ifndef _MAX3000_Lib_H_
#define _MAX3000_Lib_H_

#if defined(ARDUINO_STM32_FEATHER)
typedef class HardwareSPI SPIClass;
#endif

// Arduino / Raspberry Pi specific includes and macros
#ifndef WIRINGPI
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#else
#include <MAX3000_Pi.h>
#endif

#if defined(__AVR__)
typedef volatile uint8_t PortReg;
typedef uint8_t PortMask;
#define HAVE_PORTREG
#elif defined(__SAM3X8E__)
typedef volatile RwReg PortReg;
typedef uint32_t PortMask;
#define HAVE_PORTREG
#elif(defined(__arm__) || defined(ARDUINO_FEATHER52)) && !defined(ARDUINO_ARCH_MBED) && !defined(ARDUINO_ARCH_RP2040)
typedef volatile uint32_t PortReg;
typedef uint32_t PortMask;
#define HAVE_PORTREG
#endif

// TODO: PortReg is not working
#undef HAVE_PORTREG

#define MAX3000_DARK 0       // Draw 'off' pixels
#define MAX3000_LIGHT 1      // Draw 'on' pixels
#define MAX3000_INVERSE 2    // Invert pixels

#define MAX3000_ORDER_ROW_MAJOR 0           // Boards wired in rows
#define MAX3000_ORDER_ROW_MAJOR_BOUNCE 1    // Boards wired in rows, moving backwards on every other row
#define MAX3000_ORDER_COL_MAJOR 2           // Boards wired in columns
#define MAX3000_ORDER_COL_MAJOR_BOUNCE 3    // Boards wired in columns, moving backwards on every other column

#define PANEL_WIDTH 28     // Fixed number of columns in each MAX3000 panel
#define PANEL_HEIGHT 16    // Fixed number of rows in each MAX3000 panel

/**
 * @brief Configuration object for the MAX3000 library
 */
class MAX3000_Config {
  private:
    /**
     * @brief Default constructor.
     *
     * The width and height parameters are in pixels, and allow for chaining multiple boards.
     *
     * @param width_ If specified, sets the total width of the entire display in pixels.
     * @param height_ If specified, sets the total height of the entire display in pixels.
     */
    MAX3000_Config(uint8_t width_ = 0, uint8_t height_ = 0)
        : width(((width_ + (PANEL_WIDTH - 1)) / PANEL_WIDTH) * PANEL_WIDTH),
          height(((height_ + (PANEL_HEIGHT - 1)) / PANEL_HEIGHT) * PANEL_HEIGHT),
          boardOrder(MAX3000_ORDER_ROW_MAJOR),
          mosi_pin(-1),
          sclk_pin(-1),
          lat_pin(-1),
          rst_pin(-1),
          pulse_pin(-1),
          col_pin(-1),
          row_pin(-1),
          spi(NULL),
          spi_bitrate(65535UL) {
        numHBoards = width / PANEL_WIDTH;
        numVBoards = height / PANEL_HEIGHT;
    }

  public:
#if 0    // Disable SPI until I can figure out why it's failing to flip every bit.
    /**
     * @brief Configuration object containing a hardware SPI object.
     *
     * The width and height parameters are in pixels, and allow for chaining multiple boards.
     *
     * @param width_ If specified, sets the total width of the entire display in pixels.
     * @param height_ If specified, sets the total height of the entire display in pixels.
     * @param spi_ Hardware SPI object.
     * @param lat_pin_ Pin number connected to the MTX_LAT latch pin on the driver.
     * @param rst_pin_ Pin number connected to the MTX_RST reset pin on the driver.
     * @param pulse_pin_ Pin number connected to the PULSE_ENABLE pin on the driver.
     * @param col_pin_ Pin number connected to the COL_ENABLE_N pin on the driver.
     * @param row_pin_ Pin number connected to the ROW_ENABLE_N pin on the driver.
     */
    MAX3000_Config(uint8_t width_, uint8_t height_, SPIClass * spi_, int8_t lat_pin_, int8_t rst_pin_, int8_t pulse_pin_,
        int8_t col_pin_, int8_t row_pin_)
        : MAX3000_Config(width_, height_) {
        spi       = spi_;
        lat_pin   = lat_pin_;
        rst_pin   = rst_pin_;
        pulse_pin = pulse_pin_;
        col_pin   = col_pin_;
        row_pin   = row_pin_;
    }
#endif

    /**
     * @brief Configuration object containing software SPI pins.
     *
     * The width and height parameters are in pixels, and allow for chaining multiple boards.
     *
     * @param width_ If specified, sets the total width of the entire display in pixels.
     * @param height_ If specified, sets the total height of the entire display in pixels.
     * @param mosi_pin_ Pin number connected to the MTX_DIN data pin on the driver.
     * @param sclk_pin_ Pin number connected to the MTX_CLK clock pin on the driver.
     * @param lat_pin_ Pin number connected to the MTX_LAT latch pin on the driver.
     * @param rst_pin_ Pin number connected to the MTX_RST reset pin on the driver.
     * @param pulse_pin_ Pin number connected to the PULSE_ENABLE pin on the driver.
     * @param col_pin_ Pin number connected to the COL_ENABLE_N pin on the driver.
     * @param row_pin_ Pin number connected to the ROW_ENABLE_N pin on the driver.
     */
    MAX3000_Config(uint8_t width_, uint8_t height_, int8_t mosi_pin_, int8_t sclk_pin_, int8_t lat_pin_,
        int8_t rst_pin_, int8_t pulse_pin_, int8_t col_pin_, int8_t row_pin_)
        : MAX3000_Config(width_, height_) {
        mosi_pin  = mosi_pin_;
        sclk_pin  = sclk_pin_;
        lat_pin   = lat_pin_;
        rst_pin   = rst_pin_;
        pulse_pin = pulse_pin_;
        col_pin   = col_pin_;
        row_pin   = row_pin_;
    }

    const uint8_t width;     // Total width of the combined display.
    const uint8_t height;    // Total height of the combined display.
    uint8_t boardOrder;      // Ordering of boards within the data chain.
    int8_t mosi_pin;         // Pin connected to MTX_DIN
    int8_t sclk_pin;         // Pin connected to MTX_CLK
    int8_t lat_pin;          // Pin connected to MTX_LAT
    int8_t rst_pin;          // Pin connected to MTX_RST
    int8_t pulse_pin;        // Pin connected to PULSE_ENABLE
    int8_t col_pin;          // Pin connected to COL_ENABLE_N
    int8_t row_pin;          // Pin connected to ROW_ENABLE_N
    SPIClass * spi;          // SPI Hardware Instance
    uint32_t spi_bitrate;    // Bitrate of the SPI communications

    // Computed in constructor:
    size_t numHBoards;    // Number of horizontal boards in the total display matrix
    size_t numVBoards;    // Number of vertical boards in the total display matrix
};

/**
 * @brief Library for interacting with a MAX3000 Driver
 */
class MAX3000_Base {
  public:
    /**
     * @brief Virtual Destuctor
     */
    virtual ~MAX3000_Base(void);

    /**
     * @brief Allocate RAM for image buffer, initialize peripherals and pins.
     *
     * MUST call this function before any drawing or updates!
     *
     * @param reset If true, the MTX_RST pin will be triggered before startup.
     *              This will delay for an amount of time to ensure proper state.
     * @param periphBegin If true, the I2C and/or Hardware SPI peripherals will
     *                    have their begin() function called automatically.
     * @return Returns true on successful allocation and initialization.
     */
    bool begin(bool reset = true, bool periphBegin = true);

    /*!
        @brief  Clear contents of display buffer (set all pixels to off).
        @return None (void).
        @note   Changes buffer contents only, no immediate effect on display.
                Follow up with a call to display(), or with other graphics
                commands as needed by one's own application.
    */

    /**
     * @brief Computes pixels that have changed and sends pulses to the MAX3000 driver.
     *
     * Drawing operations are not visible until this function is
     * called. Call after each graphics command, or after a whole set
     * of graphics commands, as best needed by one's own application.
     *
     * @param force When true, sends a pulse for every pixel, instead of only
     *              pixels that have changed.
     */
    void display(bool force = false);

    /**
     * @brief Prints a text-based representation of the current buffer.
     *
     * @param stream Output stream, defaults to Serial.
     */
    void printDisplay(Stream & stream = Serial);

    /**
     * @brief Clear contents of display buffer (set all pixels to off).
     *
     * Changes buffer contents only, no immediate effect on display.
     * Follow up with a call to display(), or with other graphics
     * commands as needed by one's own application.
     */
    void clearDisplay(void);

    /**
     * @brief Sets the state of the User LED on the given board.
     *
     * @param board Board Index, starting from 0
     * @param state Whether the user LED should be on or off.
     */
    void setUserLED(size_t board, bool state);

    /**
     * @brief Return color of a single pixel in display buffer.
     *
     * Reads from buffer contents; may not reflect current contents of
     * screen if display() has not been called.
     *
     * @param x Column of display -- 0 at left to (screen width - 1) at right.
     * @param y Row of display -- 0 at top to (screen height -1) at bottom.
     * @return true if pixel is set.
     */
    bool getPixel(int16_t x, int16_t y);

    /**
     * @brief Get base address of display buffer for direct reading or writing.
     *
     * @return Pointer to an unsigned 8-bit array, column-major, columns padded
     * to full byte boundary if needed.
     */
    uint8_t * getBuffer(void);

    /**
     * @brief Enable or disable display invert mode (white-on-black vs black-on-white).
     *
     * This is also invoked by the Adafruit_GFX library in generating
     * many higher-level graphics primitives.
     *
     * This has an immediate effect on the display, no need to call the
     * display() function -- buffer contents are not changed, rather a
     * different pixel mode of the display hardware is used. When
     * enabled, drawing MAX3000_DARK (value 0) pixels will actually draw
     * white, MAX3000_LIGHT (value 1) will draw black.
     *
     * @param i If true, switch to invert mode (black-on-white), else normal mode (white-on-black).
     */
    virtual void invertDisplay(bool i);

    /**
     * @brief Set/clear/invert a single pixel.
     *
     * This is also invoked by the Adafruit_GFX library in generating
     * many higher-level graphics primitives.
     *
     * Changes buffer contents only, no immediate effect on display.
     * Follow up with a call to display(), or with other graphics
     * commands as needed by one's own application.
     *
     * @param x Column of display -- 0 at left to (screen width - 1) at right.
     * @param y Row of display -- 0 at top to (screen height -1) at bottom.
     * @param color Line color, one of: MAX3000_LIGHT, MAX3000_DARK, or MAX3000_INVERSE.
     */
    virtual void drawPixel(int16_t x, int16_t y, uint16_t color);

    /**
     * @brief Sets whether updates should be random or sequential
     *
     * If dissolve is enabled, the update order will be random and appear
     * to be "Dissolving". Otherwise, it will update in order.
     *
     * @param param Whether dissolve should be enabled
     */
    void setDissolveEnable(bool param);

    /**
     * @brief Sets the duration of each flip pulse in microseconds
     *
     * The default value should work for most applications.
     *
     * @param duration Duration of a single pulse in microseconds
     */
    void setPulseDurationUs(uint16_t duration);

    /**
     * @brief Sets whether each call to display() will take the same time
     *
     * Normally, display() will only take as long as necessary to update
     * the pixels that have changed. However if this is true, it will
     * delay as necessary so that each call takes the same time.
     * This constant frame rate is roughly equal to:
     *     28 * 16 * (pulseDuration + 10 + 290) = 246ms = 4.1fps
     *
     * @param param Whether constant frame rate should be used
     */
    void setConstantFrameRate(bool param);

  protected:
    /**
     * @brief Constructs a new MAX3000_Base object.
     *
     * Protected so it can only be used by MAX3000_GFX or MAX3000_Display.
     *
     * Call the object's begin() function before use -- buffer
     * allocation is performed there!
     *
     * @param config \ref MAX3000_Config object containing parameters for display
     */
    MAX3000_Base(const MAX3000_Config & config);

    /**
     * @brief Writes shift register buffer out to display drivers.
     *
     * See HAVE_PORTREG which defines if the method uses a port or bit-bang method.
     */
    inline void shiftRegWrite() __attribute__((always_inline));

    /**
     * @brief Stores the appropriate decoder inputs to the shift register buffer.
     *
     * @param board Board Index, starting from 0
     * @param row Row index within a single panel.
     * @param column Column index within a single panel.
     */
    void selectRowColumn(size_t board, size_t row, size_t column);

    /**
     * Controls the various pulse lines in the correct order to turn bits on
     */
    void setPixel();

    /**
     * Controls the various pulse lines in the correct order to turn bits off
     */
    void clearPixel();

    /**
     * Shuffles the internal array of indexes.
     */
    void shuffleIndex();

    /**
     * @brief Set rotation setting for display
     * @param x 0 thru 3 corresponding to 4 cardinal rotations
     */
    void setDisplayRotation(uint8_t r);

    /** @brief Configuration of display drivers */
    MAX3000_Config config;

    /** @brief Internal pixel memory buffer */
    uint8_t * buffer;

    /** @brief The previous pixel memory buffer from the last display() call. */
    uint8_t * oldBuffer;

    /** @brief Display width as modified by current rotation */
    int16_t localWidth;

    /** @brief Display height as modified by current rotation */
    int16_t localHeight;

    /** @brief Display rotation (0 thru 3) */
    uint8_t localRotation;

    /** @brief Duration of each pulse in microseconds */
    uint16_t pulseDuration;

    /** @brief Whether or not the display should be white-on-black or not */
    bool invertEnabled;

    /** @brief When enabled, bits will flip in a random order instead of sequentially */
    bool dissolveEnabled;

    /** @brief Whether constant frame rate is enabled */
    bool constantRate;

    /** @brief State flag that indicates when an update has not yet been done */
    bool firstUpdate;

    /** @brief Array of length PIXEL_HEIGHT * PIXEL_WIDTH that maps indexes to a random number */
    int * shuffledIndex;

    /** @brief Array with length of number of boards, storing the 16-bit shift register contents to send */
    uint16_t * shiftReg;

#ifdef HAVE_PORTREG
    PortReg *mosiPort, *clkPort, *latPort, *pulsePort, *rowPort, *colPort;
    PortMask mosiPinMask, clkPinMask, latPinMask, pulsePinMask, rowPinMask, colPinMask;
#endif

#if defined(SPI_HAS_TRANSACTION)
  protected:
    SPISettings spiSettings;
#endif
};

/**
 * Generic dependency-free implementation of MAX3000 Display
 *
 * Workaround for non-virtual functions width(), height(), rotation()
 * in Adafruit_GFX.
 */
class MAX3000_Display : public MAX3000_Base {
  public:
    /**
     * @brief Constructs a new MAX3000_Display object.
     *
     * Call the object's begin() function before use -- buffer
     * allocation is performed there!
     *
     * @param config \ref MAX3000_Config object containing parameters for display
     */
    MAX3000_Display(const MAX3000_Config & config)
        : MAX3000_Base(config) {
    }

    /**
     * @brief Virtual Destuctor
     */
    virtual ~MAX3000_Display(void) {}

    /**
     * @brief Get width of the display, accounting for current rotation
     * @returns Width in pixels
     */
    int16_t
    width(void) const { return localWidth; };

    /**
     * @brief Get height of the display, accounting for current rotation
     * @returns Height in pixels
     */
    int16_t height(void) const { return localHeight; }

    /**
     * @brief Get rotation setting for display
     * @returns 0 thru 3 corresponding to 4 cardinal rotations
     */
    uint8_t getRotation(void) const { return rotation; }

    /**
     * @brief Set rotation setting for display
     * @param r 0 thru 3 corresponding to 4 cardinal rotations
     */
    void setRotation(uint8_t r) {
        rotation = r;
        setDisplayRotation(rotation);
    }

  protected:
    uint8_t rotation;    ///< Display rotation (0 thru 3)
};

#endif    // _MAX3000_Lib_H_