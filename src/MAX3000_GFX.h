/**
 * @file MAX3000_GFX.h
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

#ifndef _MAX3000_GFX_H_
#define _MAX3000_GFX_H_

#include <Adafruit_GFX.h>
#include <MAX3000_Lib.h>

/**
 * @brief Wrapper around \ref MAX3000_Lib for use with Adafruit GFX Library
 */
class MAX3000_GFX : public Adafruit_GFX, public MAX3000_Base {
  public:
    /**
     * @brief Constructs a new MAX3000_GFX object.
     *
     * Call the object's begin() function before use -- buffer
     * allocation is performed there!
     *
     * @param config \ref MAX3000_Config object containing parameters for display
     */
    MAX3000_GFX(const MAX3000_Config & config);

    /**
     * @brief Virtual Destuctor
     */
    virtual ~MAX3000_GFX(void);

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
     * @brief Draw a horizontal line.
     *
     * This is also invoked by the Adafruit_GFX library in generating
     * many higher-level graphics primitives.
     *
     * Changes buffer contents only, no immediate effect on display.
     * Follow up with a call to display(), or with other graphics
     * commands as needed by one's own application.
     *
     * @param x Leftmost column -- 0 at left to (screen width - 1) at right.
     * @param y Row of display -- 0 at top to (screen height -1) at bottom.
     * @param w Width of line, in pixels.
     * @param color Line color, one of: MAX3000_LIGHT, MAX3000_DARK, or MAX3000_INVERSE.
     */
    virtual void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);

    /**
     * @brief Draw a vertical line.
     *
     * This is also invoked by the Adafruit_GFX library in generating
     * many higher-level graphics primitives.
     *
     * Changes buffer contents only, no immediate effect on display.
     * Follow up with a call to display(), or with other graphics
     * commands as needed by one's own application.
     *
     * @param x Column of display -- 0 at left to (screen width -1) at right.
     * @param y Topmost row -- 0 at top to (screen height - 1) at bottom.
     * @param h Height of line, in pixels.
     * @param color Line color, one of: MAX3000_LIGHT, MAX3000_DARK, or MAX3000_INVERSE.
     */
    virtual void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);

    /**
     * @brief Set rotation setting for display
     * @param r 0 thru 3 corresponding to 4 cardinal rotations
     */
    void setRotation(uint8_t r) {
        rotation = r;
        setDisplayRotation(rotation);
    }

  protected:
    /**
     * @brief Draw a horizontal line with a width and color.
     *
     * Used by public methods \ref drawFastHLine and \ref drawFastVLine.
     *
     * Changes buffer contents only, no immediate effect on display.
     * Follow up with a call to display(), or with other graphics
     * commands as needed by one's own application.
     *
     * @param x Leftmost column -- 0 at left to (screen width - 1) at right.
     * @param y Row of display -- 0 at top to (screen height -1) at bottom.
     * @param w Width of line, in pixels.
     * @param color Line color, one of: MAX3000_LIGHT, MAX3000_DARK, or MAX3000_INVERSE.
     */
    void drawFastHLineInternal(int16_t x, int16_t y, int16_t w, uint16_t color);

    /**
     * @brief Draw a vertical line with a width and color.
     *
     * Used by public methods \ref drawFastHLine and \ref drawFastVLine.
     *
     * Changes buffer contents only, no immediate effect on display.
     * Follow up with a call to display(), or with other graphics
     * commands as needed by one's own application.
     *
     * @param x Column of display -- 0 at left to (screen width -1) at right.
     * @param y Topmost row -- 0 at top to (screen height - 1) at bottom.
     * @param h Height of line, in pixels.
     * @param color Line color, one of: MAX3000_LIGHT, MAX3000_DARK, or MAX3000_INVERSE.
     */
    void drawFastVLineInternal(int16_t x, int16_t y, int16_t h, uint16_t color);
};

#endif    // _MAX3000_GFX_H_