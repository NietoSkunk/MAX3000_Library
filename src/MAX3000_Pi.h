#ifndef _MAX3000_Pi_H_
#define _MAX3000_Pi_H_

#include <wiringPi.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <string>

typedef std::string String;

class SPIClass {
  public:
    void begin() {}
    uint16_t transfer16(uint16_t data) { return 0; }
};

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2

class Stream {
  private:
    int write_error;
    size_t printNumber(unsigned long, uint8_t);
    size_t printFloat(double, uint8_t);

  protected:
    void setWriteError(int err = 1) { write_error = err; }

  public:
    Stream()
        : write_error(0) {}

    bool begin(int baud) { return true; }

    int getWriteError() { return write_error; }
    void clearWriteError() { setWriteError(0); }

    virtual size_t write(uint8_t);
    size_t write(const char * str) {
        if(str == NULL) return 0;
        return write((const uint8_t *)str, strlen(str));
    }
    virtual size_t write(const uint8_t * buffer, size_t size);
    size_t write(const char * buffer, size_t size) {
        return write((const uint8_t *)buffer, size);
    }

    // default to zero, meaning "a single write may block"
    // should be overridden by subclasses with buffering
    virtual int availableForWrite() { return 0; }

    size_t print(const String &);
    size_t print(const char[]);
    size_t print(char);
    size_t print(unsigned char, int = DEC);
    size_t print(int, int = DEC);
    size_t print(unsigned int, int = DEC);
    size_t print(long, int = DEC);
    size_t print(unsigned long, int = DEC);
    size_t print(double, int = 2);

    size_t println(const String & s);
    size_t println(const char[]);
    size_t println(char);
    size_t println(unsigned char, int = DEC);
    size_t println(int, int = DEC);
    size_t println(unsigned int, int = DEC);
    size_t println(long, int = DEC);
    size_t println(unsigned long, int = DEC);
    size_t println(double, int = 2);
    size_t println(void);

    virtual void flush() { /* Empty implementation for backward compatibility */
    }
};

extern Stream Serial;

#endif