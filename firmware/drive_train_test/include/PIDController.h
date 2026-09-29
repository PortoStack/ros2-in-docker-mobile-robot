#pragma once
#include <Arduino.h>

class PIDController {
public:
    PIDController(float kp, float ki, float kd, float minOutput, float maxOutput, float integralLimit);

    void reset();
    float compute(float target, float measured, float dt);
    void setTunings(float kp, float ki, float kd);

private:
    float _kp;
    float _ki;
    float _kd;
    float _minOutput;
    float _maxOutput;
    float _integralLimit;

    float _integral;
    float _lastError;
};
