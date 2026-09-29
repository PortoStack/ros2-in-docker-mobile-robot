#include "Robot.h"

Robot::Robot(
    Motor& leftMotor,
    Motor& rightMotor,
    Encoder& leftEncoder,
    Encoder& rightEncoder
) : _leftMotor(leftMotor),
    _rightMotor(rightMotor),
    _leftEncoder(leftEncoder),
    _rightEncoder(rightEncoder),
    _leftPID(PID_KP, PID_KI, PID_KD, PID_MIN_PWM, PID_MAX_PWM, PID_INTEGRAL_LIMIT),
    _rightPID(PID_KP, PID_KI, PID_KD, PID_MIN_PWM, PID_MAX_PWM, PID_INTEGRAL_LIMIT),
    _kinematics(TRACK_WIDTH_M, METERS_PER_TICK),
    _targetSpeeds{0.0f, 0.0f}
{
}

void Robot::begin() {
    _leftMotor.begin();
    _rightMotor.begin();
    _leftEncoder.begin();
    _rightEncoder.begin();
    stop();
}

void Robot::setTargetVelocity(float linearX, float angularZ) {
    _targetSpeeds = _kinematics.inverseKinematics(linearX, angularZ);
}

void Robot::update(float dt) {
    if (dt <= 0.0f) {
        return;
    }

    // 1. Measure actual wheel speeds from encoders (m/s)
    float measuredLeftSpeed = _leftEncoder.getSpeedMetersPerSec(dt);
    float measuredRightSpeed = _rightEncoder.getSpeedMetersPerSec(dt);

    // 2. Compute PID PWM control outputs
    float leftPWM = _leftPID.compute(_targetSpeeds.left, measuredLeftSpeed, dt);
    float rightPWM = _rightPID.compute(_targetSpeeds.right, measuredRightSpeed, dt);

    // 3. Actuate motors
    _leftMotor.setSpeed((int)leftPWM);
    _rightMotor.setSpeed((int)rightPWM);

    // 4. Update Odometry from cumulative encoder counts
    int64_t leftTicks = _leftEncoder.getCount();
    int64_t rightTicks = _rightEncoder.getCount();
    _kinematics.updateOdometry(leftTicks, rightTicks, dt);
}

void Robot::stop() {
    _targetSpeeds.left = 0.0f;
    _targetSpeeds.right = 0.0f;
    _leftPID.reset();
    _rightPID.reset();
    _leftMotor.stop();
    _rightMotor.stop();
}

const OdometryData& Robot::getOdometry() const {
    return _kinematics.getOdometry();
}

void Robot::resetOdometry() {
    _leftEncoder.reset();
    _rightEncoder.reset();
    _kinematics.reset();
}