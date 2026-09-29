import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
import xacro

def generate_launch_description():
    pkg_description = get_package_share_directory('medbot_description')
    pkg_bringup = get_package_share_directory('medbot_bringup')

    # Launch Configurations & Arguments
    serial_port_arg = DeclareLaunchArgument(
        'serial_port',
        default_value='/dev/ttyUSB0',
        description='Serial port for ESP32 micro-ROS agent'
    )
    baudrate_arg = DeclareLaunchArgument(
        'baudrate',
        default_value='115200',
        description='Baudrate for ESP32 micro-ROS agent'
    )
    lidar_port_arg = DeclareLaunchArgument(
        'lidar_port',
        default_value='/dev/ttyUSB1',
        description='Serial port for YDLidar'
    )
    use_foxglove_arg = DeclareLaunchArgument(
        'use_foxglove',
        default_value='true',
        description='Launch Foxglove Bridge for remote visualization (port 8765)'
    )

    serial_port = LaunchConfiguration('serial_port')
    baudrate = LaunchConfiguration('baudrate')
    use_foxglove = LaunchConfiguration('use_foxglove')

    # Process URDF
    xacro_file = os.path.join(pkg_description, 'urdf', 'medbot.urdf.xacro')
    robot_description_raw = xacro.process_file(xacro_file).toxml()

    # Robot State Publisher
    robot_state_publisher_node = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        name='robot_state_publisher',
        output='screen',
        parameters=[{'robot_description': robot_description_raw, 'use_sim_time': False}]
    )

    # micro-ROS Agent (Communication with ESP32)
    micro_ros_agent_node = Node(
        package='micro_ros_agent',
        executable='micro_ros_agent',
        name='micro_ros_agent',
        arguments=['serial', '--dev', serial_port, '-b', baudrate],
        output='screen'
    )

    # Joy Node
    joy_node = Node(
        package='joy',
        executable='joy_node',
        name='joy_node',
        parameters=[{'autorepeat_rate': 20.0}]
    )

    # Teleop Twist Joy (publishes to /cmd_vel_joy)
    teleop_twist_joy_node = Node(
        package='teleop_twist_joy',
        executable='teleop_twist_joy_node',
        name='teleop_twist_joy_node',
        parameters=[os.path.join(pkg_bringup, 'config', 'joy_params.yaml')],
        remappings=[('/cmd_vel', '/cmd_vel_joy')]
    )

    # Twist Mux (multiplexes /cmd_vel_joy and /cmd_vel_nav to /cmd_vel)
    twist_mux_node = Node(
        package='twist_mux',
        executable='twist_mux',
        name='twist_mux',
        parameters=[os.path.join(pkg_bringup, 'config', 'twist_mux.yaml')],
        remappings=[('/cmd_vel_out', '/cmd_vel')]
    )

    # YDLIDAR Driver
    ydlidar_node = Node(
        package='ydlidar_ros2_driver',
        executable='ydlidar_ros2_driver_node',
        name='ydlidar_ros2_driver_node',
        parameters=[os.path.join(pkg_bringup, 'config', 'ydlidar_params.yaml')],
        output='screen'
    )

    # Foxglove Bridge (WebSockets for Foxglove Studio UI / Visualization)
    foxglove_bridge_node = Node(
        package='foxglove_bridge',
        executable='foxglove_bridge',
        name='foxglove_bridge',
        parameters=[{'port': 8765, 'send_buffer_limit': 10000000}],
        output='screen',
        condition=IfCondition(use_foxglove)
    )

    return LaunchDescription([
        serial_port_arg,
        baudrate_arg,
        lidar_port_arg,
        use_foxglove_arg,
        robot_state_publisher_node,
        micro_ros_agent_node,
        joy_node,
        teleop_twist_joy_node,
        twist_mux_node,
        ydlidar_node,
        foxglove_bridge_node
    ])
