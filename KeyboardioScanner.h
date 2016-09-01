#pragma once

#include <Arduino.h>
#include "wire-protocol-constants.h"

struct cRGB {
    uint8_t b;
    uint8_t g;
    uint8_t r;
};

#define LED_BANKS 4

#define LED_COUNT 32
#define LED_BYTES_PER_BANK sizeof(cRGB)  * LED_COUNT/LED_BANKS

typedef union LEDData_t {
    cRGB leds[LED_COUNT];
    byte bytes[LED_BANKS][LED_BYTES_PER_BANK];
};


// Same datastructure as on the other side
typedef union {
    struct {
        uint8_t row:2,
                col:3,
                keyState:1,
                keyEventsWaiting:1,
                eventReported:1;
    };
    uint8_t val;
} key_t;


typedef union {
    uint8_t rows[4];
    uint32_t all;
} keydata_t;

// config options

// used to configure interrupts, configuration for a particular controller
class KeyboardioScanner {
  public:
    KeyboardioScanner(byte setAd01);
    ~KeyboardioScanner();
    
    int readVersion();
    
    byte setKeyscanInterval(byte delay);
    int readKeyscanInterval();

    byte setLEDSPIFrequency(byte frequency);
    int readLEDSPIFrequency();

    bool moreKeysWaiting();
    void sendLEDData();
    void setOneLEDTo(byte led, cRGB color);
    void setAllLEDsTo(cRGB color);
    keydata_t getKeyData();
    bool readKeys();
    LEDData_t ledData;
    uint8_t controllerAddress();

  private:
    bool configured = false;
    int addr;
    int ad01;
    keydata_t keyData;
    bool keyReady = false;
    byte nextLEDBank = 0;
    void sendLEDBank(byte bank);
    int readRegister(int cmd);
};

