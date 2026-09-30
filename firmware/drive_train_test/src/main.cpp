#include <Arduino.h>
#include <micro_ros_platformio.h>

#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <rmw_microros/rmw_microros.h>

#include <geometry_msgs/msg/twist.h>
#include <nav_msgs/msg/odometry.h>

#include "Config.h"
#include "Motor.h"
#include "Encoder.h"
#include "Robot.h"

// ========================================
// Hardware Instances
// ========================================
Motor leftMotor(LEFT_MOTOR_RPWM, LEFT_MOTOR_LPWM, LEFT_PWM_CHANNEL_R, LEFT_PWM_CHANNEL_L);
Motor rightMotor(RIGHT_MOTOR_RPWM, RIGHT_MOTOR_LPWM, RIGHT_PWM_CHANNEL_R, RIGHT_PWM_CHANNEL_L);

Encoder leftEncoder(LEFT_ENCODER_A, LEFT_ENCODER_B);
Encoder rightEncoder(RIGHT_ENCODER_A, RIGHT_ENCODER_B);

Robot robot(leftMotor, rightMotor, leftEncoder, rightEncoder);

// ========================================
// micro-ROS Entities & Messages
// ========================================
rcl_allocator_t allocator;
rclc_support_t support;
rcl_node_t node;
rclc_executor_t executor;
rcl_timer_t timer;

rcl_subscription_t twist_subscriber;
geometry_msgs__msg__Twist twist_msg;

rcl_publisher_t odom_publisher;
nav_msgs__msg__Odometry odom_msg;

// ========================================
// State Variables
// ========================================
enum AgentState {
    WAITING_AGENT,
    AGENT_AVAILABLE,
    AGENT_CONNECTED,
    AGENT_DISCONNECTED
} agent_state;

unsigned long last_cmd_vel_time = 0;
unsigned long last_control_time = 0;
float target_vx = 0.0f;
float target_wz = 0.0f;

// ========================================
// Callbacks
// ========================================

// /cmd_vel Callback
void cmd_vel_callback(const void *msgin) {
    const geometry_msgs__msg__Twist *msg = (const geometry_msgs__msg__Twist *)msgin;
    target_vx = (float)msg->linear.x;
    target_wz = (float)msg->angular.z;
    last_cmd_vel_time = millis();
}

// Control Loop & Odometry Timer Callback (50 Hz)
void timer_callback(rcl_timer_t *timer, int64_t last_call_time) {
    RCLC_UNUSED(last_call_time);
    if (timer == NULL) {
        return;
    }

    unsigned long current_time = millis();
    float dt = (current_time - last_control_time) / 1000.0f;
    if (dt <= 0.0f || dt > 0.5f) {
        dt = 1.0f / CONTROL_LOOP_FREQ_HZ;
    }
    last_control_time = current_time;

    // Safety Watchdog: stop if no command received within timeout
    if (current_time - last_cmd_vel_time > CMD_VEL_TIMEOUT_MS) {
        robot.stop();
    } else {
        robot.setTargetVelocity(target_vx, target_wz);
    }

    // Update PID & Odometry
    robot.update(dt);

    // Populate Odometry Message
    const OdometryData& odom = robot.getOdometry();

    // Timestamp
    int64_t time_ns = rmw_uros_epoch_nanos();
    odom_msg.header.stamp.sec = (int32_t)(time_ns / 1000000000);
    odom_msg.header.stamp.nanosec = (uint32_t)(time_ns % 1000000000);

    // Frame IDs
    odom_msg.header.frame_id.data = (char *)ODOM_FRAME_ID;
    odom_msg.header.frame_id.size = strlen(ODOM_FRAME_ID);
    odom_msg.child_frame_id.data = (char *)BASE_FRAME_ID;
    odom_msg.child_frame_id.size = strlen(BASE_FRAME_ID);

    // Pose
    odom_msg.pose.pose.position.x = odom.x;
    odom_msg.pose.pose.position.y = odom.y;
    odom_msg.pose.pose.position.z = 0.0;

    odom_msg.pose.pose.orientation.x = odom.qx;
    odom_msg.pose.pose.orientation.y = odom.qy;
    odom_msg.pose.pose.orientation.z = odom.qz;
    odom_msg.pose.pose.orientation.w = odom.qw;

    // Twist
    odom_msg.twist.twist.linear.x = odom.vx;
    odom_msg.twist.twist.linear.y = 0.0;
    odom_msg.twist.twist.linear.z = 0.0;
    odom_msg.twist.twist.angular.x = 0.0;
    odom_msg.twist.twist.angular.y = 0.0;
    odom_msg.twist.twist.angular.z = odom.vtheta;

    // Covariance matrices
    for (int i = 0; i < 36; i++) {
        odom_msg.pose.covariance[i] = 0.0;
        odom_msg.twist.covariance[i] = 0.0;
    }
    // Pose covariance diagonal
    odom_msg.pose.covariance[0] = 0.001;  // x
    odom_msg.pose.covariance[7] = 0.001;  // y
    odom_msg.pose.covariance[14] = 1e6;   // z (untracked)
    odom_msg.pose.covariance[21] = 1e6;   // roll (untracked)
    odom_msg.pose.covariance[28] = 1e6;   // pitch (untracked)
    odom_msg.pose.covariance[35] = 0.01;  // yaw

    // Twist covariance diagonal
    odom_msg.twist.covariance[0] = 0.001; // vx
    odom_msg.twist.covariance[7] = 1e6;   // vy (untracked)
    odom_msg.twist.covariance[14] = 1e6;  // vz (untracked)
    odom_msg.twist.covariance[21] = 1e6;  // roll rate
    odom_msg.twist.covariance[28] = 1e6;  // pitch rate
    odom_msg.twist.covariance[35] = 0.01; // wz

    // Publish /odom
    rcl_publish(&odom_publisher, &odom_msg, NULL);
}

// ========================================
// micro-ROS Lifecycle Management
// ========================================
bool create_entities() {
    allocator = rcl_get_default_allocator();

    rclc_support_init(&support, 0, NULL, &allocator);

    rclc_node_init_default(&node, NODE_NAME, "", &support);

    rclc_subscription_init_default(
        &twist_subscriber,
        &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist),
        CMD_VEL_TOPIC
    );

    rclc_publisher_init_default(
        &odom_publisher,
        &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(nav_msgs, msg, Odometry),
        ODOM_TOPIC
    );

    rclc_timer_init_default(
        &timer,
        &support,
        RCL_MS_TO_NS(CONTROL_LOOP_DT_MS),
        timer_callback
    );

    rclc_executor_init(&executor, &support.context, 2, &allocator);
    rclc_executor_add_subscription(&executor, &twist_subscriber, &twist_msg, &cmd_vel_callback, ON_NEW_DATA);
    rclc_executor_add_timer(&executor, &timer);

    // Synchronize time with agent
    rmw_uros_sync_session(1000);

    return true;
}

void destroy_entities() {
    rmw_context_t *rmw_context = rcl_context_get_rmw_context(&support.context);
    (void)rmw_uros_set_context_entity_destroy_session_timeout(rmw_context, 0);

    rcl_publisher_fini(&odom_publisher, &node);
    rcl_subscription_fini(&twist_subscriber, &node);
    rcl_timer_fini(&timer);
    rclc_executor_fini(&executor);
    rcl_node_fini(&node);
    rclc_support_fini(&support);
}

// ========================================
// Setup & Loop
// ========================================
void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
    set_microros_serial_transports(Serial);

    robot.begin();

    agent_state = WAITING_AGENT;
    last_control_time = millis();
}

unsigned long last_ping_time = 0;

void loop() {
    switch (agent_state) {
        case WAITING_AGENT:
            // Check if agent is available over serial
            if (rmw_uros_ping_agent(100, 1) == RMW_RET_OK) {
                agent_state = AGENT_AVAILABLE;
            }
            break;

        case AGENT_AVAILABLE:
            if (create_entities()) {
                agent_state = AGENT_CONNECTED;
                last_ping_time = millis();
            } else {
                agent_state = WAITING_AGENT;
            }
            break;

        case AGENT_CONNECTED:
            // Real-time non-blocking executor spin
            rclc_executor_spin_some(&executor, RCL_MS_TO_NS(2));

            // Periodic heartbeat check every 2 seconds without blocking loop
            if (millis() - last_ping_time > 2000) {
                last_ping_time = millis();
                if (rmw_uros_ping_agent(50, 1) != RMW_RET_OK) {
                    agent_state = AGENT_DISCONNECTED;
                }
            }
            break;

        case AGENT_DISCONNECTED:
            robot.stop();
            destroy_entities();
            agent_state = WAITING_AGENT;
            break;

        default:
            break;
    }
}