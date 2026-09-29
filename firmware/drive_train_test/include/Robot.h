#pragma once
#include <Arduino.h>
#include "Motor.h"
#include "Encoder.h"
#include "PIDController.h"
#include "Kinematics.h"
#include "Config.h"

class Robot {
public:
    Robot(
        Motor& leftMotor,
        Motor& rightMotor,
        Encoder& leftEncoder,
        Encoder& rightEncoder
    );

    void begin();
    void setTargetVelocity(float linearX, float angularZ);
    void update(float dt);
    void stop();

    const OdometryData& getOdometry() const;
    void resetOdometry();

private:
    Motor& _leftMotor;
    Motor& _rightMotor;
    Encoder& _leftEncoder;
    Encoder& _rightEncoder;

    PIDController _leftPID;
    PIDController _rightPID;
    Kinematics _kinematics;

    WheelSpeeds _targetSpeeds;
};