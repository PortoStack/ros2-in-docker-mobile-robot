#ifndef MOTOR_H
#define MOTOR_H


class Motor
{
public:

    Motor(
        int rpwmPin,
        int lpwmPin,
        int channelRPWM,
        int channelLPWM
    );

    void begin();

    void setSpeed(
        int speed
    );

    void stop();

private:

    int _rpwmPin;
    int _lpwmPin;

    int _channelRPWM;
    int _channelLPWM;
};

#endif