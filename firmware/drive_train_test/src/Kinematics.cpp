#include "Kinematics.h"
#include <math.h>

Kinematics::Kinematics(float trackWidth, float metersPerTick)
    : _trackWidth(trackWidth),
      _metersPerTick(metersPerTick),
      _lastLeftTicks(0),
      _lastRightTicks(0)
{
    reset();
}

void Kinematics::reset() {
    _odom.x = 0.0f;
    _odom.y = 0.0f;
    _odom.theta = 0.0f;
    _odom.qx = 0.0f;
    _odom.qy = 0.0f;
    _odom.qz = 0.0f;
    _odom.qw = 1.0f;
    _odom.vx = 0.0f;
    _odom.vtheta = 0.0f;
    _lastLeftTicks = 0;
    _lastRightTicks = 0;
}

WheelSpeeds Kinematics::inverseKinematics(float linearX, float angularZ) {
    WheelSpeeds speeds;
    // vL = vx - (w * W / 2)
    // vR = vx + (w * W / 2)
    speeds.left = linearX - (angularZ * _trackWidth / 2.0f);
    speeds.right = linearX + (angularZ * _trackWidth / 2.0f);
    return speeds;
}

void Kinematics::updateOdometry(int64_t leftTicks, int64_t rightTicks, float dt) {
    if (dt <= 0.0f) {
        return;
    }

    int64_t deltaLeft = leftTicks - _lastLeftTicks;
    int64_t deltaRight = rightTicks - _lastRightTicks;

    _lastLeftTicks = leftTicks;
    _lastRightTicks = rightTicks;

    float distLeft = (float)deltaLeft * _metersPerTick;
    float distRight = (float)deltaRight * _metersPerTick;

    float deltaDistance = (distRight + distLeft) / 2.0f;
    float deltaTheta = (distRight - distLeft) / _trackWidth;

    // Runge-Kutta / Midpoint Integration
    float midTheta = _odom.theta + (deltaTheta / 2.0f);
    _odom.x += deltaDistance * cosf(midTheta);
    _odom.y += deltaDistance * sinf(midTheta);
    _odom.theta = normalizeAngle(_odom.theta + deltaTheta);

    // Compute velocities
    _odom.vx = deltaDistance / dt;
    _odom.vtheta = deltaTheta / dt;

    // Compute Quaternion for ROS 2 (rotation purely around Z)
    _odom.qx = 0.0f;
    _odom.qy = 0.0f;
    _odom.qz = sinf(_odom.theta / 2.0f);
    _odom.qw = cosf(_odom.theta / 2.0f);
}

const OdometryData& Kinematics::getOdometry() const {
    return _odom;
}

float Kinematics::normalizeAngle(float angle) {
    while (angle > M_PI)  angle -= 2.0f * M_PI;
    while (angle < -M_PI) angle += 2.0f * M_PI;
    return angle;
}
