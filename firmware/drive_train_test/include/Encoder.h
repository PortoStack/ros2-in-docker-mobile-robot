#pragma once
#include <Arduino.h>

class Encoder {
public:
    Encoder(uint8_t pinA, uint8_t pinB);

    void begin();
    int64_t getCount();
    void reset();
    
    // Returns speed in meters per second (m/s)
    float getSpeedMetersPerSec(float dt);

private:
    uint8_t _pinA;
    uint8_t _pinB;
    uint8_t _index;

    volatile int64_t _count;
    volatile uint8_t _lastEncoded;

    int64_t _lastCount;

    void handleInterrupt();

    static Encoder* _encoder0;
    static Encoder* _encoder1;

    static void IRAM_ATTR isr0();
    static void IRAM_ATTR isr1();
};