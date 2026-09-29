#pragma once
#include <Arduino.h>

struct WheelSpeeds {
    float left;   // m/s
    float right;  // m/s
};

struct OdometryData {
    // 2D Pose
    float x;      // meters
    float y;      // meters
    float theta;  // radians

    // Quaternion (Z-axis rotation)
    float qx;
    float qy;
    float qz;
    float qw;

    // Velocities
    float vx;     // m/s
    float vtheta; // rad/s
};

class Kinematics {
public:
    Kinematics(float trackWidth, float metersPerTick);

    WheelSpeeds inverseKinematics(float linearX, float angularZ);
    void updateOdometry(int64_t leftTicks, int64_t rightTicks, float dt);
    const OdometryData& getOdometry() const;
    void reset();

private:
    float _trackWidth;
    float _metersPerTick;

    int64_t _lastLeftTicks;
    int64_t _lastRightTicks;

    OdometryData _odom;

    float normalizeAngle(float angle);
};
