#pragma once
#include <Arduino.h>

// ==========================================
// Serial & micro-ROS Settings
// ==========================================
#define SERIAL_BAUD_RATE      115200
#define NODE_NAME             "esp32_base_node"
#define CMD_VEL_TOPIC         "/cmd_vel"
#define ODOM_TOPIC            "/odom"
#define ODOM_FRAME_ID         "odom"
#define BASE_FRAME_ID         "base_footprint"

// ==========================================
// Left Motor - BTS7960 Driver Pins
// ==========================================
#define LEFT_MOTOR_RPWM       25
#define LEFT_MOTOR_LPWM       26

// ==========================================
// Right Motor - BTS7960 Driver Pins
// ==========================================
#define RIGHT_MOTOR_RPWM      27
#define RIGHT_MOTOR_LPWM      14

// ==========================================
// Left Encoder Pins
// ==========================================
#define LEFT_ENCODER_A        23
#define LEFT_ENCODER_B        22

// ==========================================
// Right Encoder Pins
// ==========================================
#define RIGHT_ENCODER_A       19
#define RIGHT_ENCODER_B       18

// ==========================================
// Motor PWM Configuration (LEDC)
// ==========================================
#define PWM_FREQUENCY         20000 // 20 kHz (ultrasonic, quiet)
#define PWM_RESOLUTION        8     // 8-bit (0 - 255)

#define LEFT_PWM_CHANNEL_R    0
#define LEFT_PWM_CHANNEL_L    1
#define RIGHT_PWM_CHANNEL_R   2
#define RIGHT_PWM_CHANNEL_L   3

// ==========================================
// Robot Physical Dimensions
// ==========================================
// Wheel diameter: 9 inch = 228.6 mm = 0.2286 m
#define WHEEL_DIAMETER_M      0.2286f
#define WHEEL_RADIUS_M        (WHEEL_DIAMETER_M / 2.0f) // 0.1143 m

// Track width (distance between left and right wheel contact points): 300 mm = 0.30 m
#define TRACK_WIDTH_M         0.30f

// ==========================================
// Encoder & Drivetrain Specifications
// ==========================================
// JGB37-520 Motor: 11 PPR on motor shaft -> 44 CPR with 4x quadrature decoding
// Internal Gearbox: 20:1
// Timing Belt Reduction: 1:3 (3:1 speed reduction)
// Total Gear Reduction = 20 * 3 = 60
// Total Pulses per 1 Wheel Revolution = 44 * 60 = 2640 ticks
#define TICKS_PER_REV         2640.0f

// Distance traversed per single tick (meters)
#define METERS_PER_TICK       ((2.0f * 3.14159265358979323846f * WHEEL_RADIUS_M) / TICKS_PER_REV)

// ==========================================
// Control Loop & Safety Watchdog
// ==========================================
#define CONTROL_LOOP_FREQ_HZ  50
#define CONTROL_LOOP_DT_MS    (1000 / CONTROL_LOOP_FREQ_HZ)

// Safety Watchdog: Stop robot if no cmd_vel received within this duration (ms)
#define CMD_VEL_TIMEOUT_MS    500

// ==========================================
// Velocity Feed-Forward & PID Controller Constants
// ==========================================
#define MAX_ROBOT_SPEED_MPS   1.0f  // Reference max speed in m/s (~255 PWM at full power)
#define MIN_START_PWM         35.0f // Static friction deadband compensation

#define PID_KP                80.0f
#define PID_KI                10.0f
#define PID_KD                0.0f
#define PID_MAX_PWM           200
#define PID_MIN_PWM          -200
#define PID_INTEGRAL_LIMIT    50.0f