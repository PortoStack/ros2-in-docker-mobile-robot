#include <Arduino.h>

#include "Motor.h"


Motor::Motor(
    int rpwmPin,
    int lpwmPin,
    int channelRPWM,
    int channelLPWM
)
    : _rpwmPin(rpwmPin),
      _lpwmPin(lpwmPin),
      _channelRPWM(channelRPWM),
      _channelLPWM(channelLPWM)
{
}


void Motor::begin()
{
    ledcSetup(
        _channelRPWM,
        20000,
        8
    );

    ledcSetup(
        _channelLPWM,
        20000,
        8
    );


    ledcAttachPin(
        _rpwmPin,
        _channelRPWM
    );

    ledcAttachPin(
        _lpwmPin,
        _channelLPWM
    );


    stop();
}


void Motor::setSpeed(
    int speed
)
{
    speed = constrain(
        speed,
        -255,
        255
    );


    if (speed > 0)
    {
        // Forward

        ledcWrite(
            _channelRPWM,
            speed
        );

        ledcWrite(
            _channelLPWM,
            0
        );
    }
    else if (speed < 0)
    {
        // Backward

        ledcWrite(
            _channelRPWM,
            0
        );

        ledcWrite(
            _channelLPWM,
            -speed
        );
    }
    else
    {
        stop();
    }
}


void Motor::stop()
{
    ledcWrite(
        _channelRPWM,
        0
    );

    ledcWrite(
        _channelLPWM,
        0
    );
}