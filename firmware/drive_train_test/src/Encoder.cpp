#include "Encoder.h"
#include "Config.h"

Encoder* Encoder::_encoder0 = nullptr;
Encoder* Encoder::_encoder1 = nullptr;

// 4x Quadrature lookup table for valid state transitions
// Index = (last_A << 3) | (last_B << 2) | (curr_A << 1) | curr_B
static const int8_t QUAD_TABLE[16] = {
     0, -1,  1,  0,
     1,  0,  0, -1,
    -1,  0,  0,  1,
     0,  1, -1,  0
};

Encoder::Encoder(uint8_t pinA, uint8_t pinB)
    : _pinA(pinA),
      _pinB(pinB),
      _count(0),
      _lastEncoded(0),
      _lastCount(0)
{
    if (_encoder0 == nullptr) {
        _encoder0 = this;
        _index = 0;
    } else {
        _encoder1 = this;
        _index = 1;
    }
}

void Encoder::begin() {
    pinMode(_pinA, INPUT_PULLUP);
    pinMode(_pinB, INPUT_PULLUP);

    uint8_t a = digitalRead(_pinA);
    uint8_t b = digitalRead(_pinB);
    _lastEncoded = (a << 1) | b;

    if (_index == 0) {
        attachInterrupt(digitalPinToInterrupt(_pinA), isr0, CHANGE);
        attachInterrupt(digitalPinToInterrupt(_pinB), isr0, CHANGE);
    } else {
        attachInterrupt(digitalPinToInterrupt(_pinA), isr1, CHANGE);
        attachInterrupt(digitalPinToInterrupt(_pinB), isr1, CHANGE);
    }
}

void IRAM_ATTR Encoder::isr0() {
    if (_encoder0 != nullptr) {
        _encoder0->handleInterrupt();
    }
}

void IRAM_ATTR Encoder::isr1() {
    if (_encoder1 != nullptr) {
        _encoder1->handleInterrupt();
    }
}

void IRAM_ATTR Encoder::handleInterrupt() {
    uint8_t a = digitalRead(_pinA);
    uint8_t b = digitalRead(_pinB);
    uint8_t encoded = (a << 1) | b;
    uint8_t transition = (_lastEncoded << 2) | encoded;

    _count += QUAD_TABLE[transition & 0x0F];
    _lastEncoded = encoded;
}

int64_t Encoder::getCount() {
    noInterrupts();
    int64_t count = _count;
    interrupts();
    return count;
}

void Encoder::reset() {
    noInterrupts();
    _count = 0;
    interrupts();
    _lastCount = 0;
}

float Encoder::getSpeedMetersPerSec(float dt) {
    if (dt <= 0.0f) {
        return 0.0f;
    }

    int64_t currentCount = getCount();
    int64_t deltaTicks = currentCount - _lastCount;
    _lastCount = currentCount;

    float distanceMeters = (float)deltaTicks * METERS_PER_TICK;
    return distanceMeters / dt;
}