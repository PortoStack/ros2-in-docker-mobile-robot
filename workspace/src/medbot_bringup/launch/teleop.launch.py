import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    pkg_bringup = get_package_share_directory('medbot_bringup')
    joy_params_file = os.path.join(pkg_bringup, 'config', 'joy_params.yaml')

    joy_node = Node(
        package='joy',
        executable='joy_node',
        name='joy_node',
        parameters=[{'autorepeat_rate': 20.0}],
        output='screen'
    )

    teleop_twist_joy_node = Node(
        package='teleop_twist_joy',
        executable='teleop_node',
        name='teleop_twist_joy_node',
        parameters=[joy_params_file],
        remappings=[('/cmd_vel', '/cmd_vel_joy')],
        output='screen'
    )

    return LaunchDescription([
        joy_node,
        teleop_twist_joy_node
    ])
