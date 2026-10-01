#include "PIDController.h"
#include "Config.h"

PIDController::PIDController(
    float kp,
    float ki,
    float kd,
    float minOutput,
    float maxOutput,
    float integralLimit
) : _kp(kp),
    _ki(ki),
    _kd(kd),
    _minOutput(minOutput),
    _maxOutput(maxOutput),
    _integralLimit(integralLimit),
    _integral(0.0f),
    _lastError(0.0f)
{
}

void PIDController::reset() {
    _integral = 0.0f;
    _lastError = 0.0f;
}

float PIDController::compute(float target, float measured, float dt) {
    if (dt <= 0.0f) {
        return 0.0f;
    }

    // Deadband if target is 0
    if (fabs(target) < 1e-4f) {
        reset();
        return 0.0f;
    }

    float error = target - measured;

    // Proportional term
    float pTerm = _kp * error;

    // Integral term with anti-windup clamping
    _integral += error * dt;
    _integral = constrain(_integral, -_integralLimit, _integralLimit);
    float iTerm = _ki * _integral;

    // Derivative term (Kd=0 disables spikes from discrete encoder differentiation)
    float derivative = (error - _lastError) / dt;
    float dTerm = _kd * derivative;
    _lastError = error;

    // Feed-Forward (Linear Speed Estimation) + Static Friction Deadband
    // Provides immediate base torque so both motors start simultaneously without waiting for integral windup
    float feedForward = (target / MAX_ROBOT_SPEED_MPS) * (PID_MAX_PWM - MIN_START_PWM);
    if (target > 0.005f) {
        feedForward += MIN_START_PWM;
    } else if (target < -0.005f) {
        feedForward -= MIN_START_PWM;
    }

    // Compute total control output
    float output = feedForward + pTerm + iTerm + dTerm;
    return constrain(output, _minOutput, _maxOutput);
}

void PIDController::setTunings(float kp, float ki, float kd) {
    _kp = kp;
    _ki = ki;
    _kd = kd;
}
