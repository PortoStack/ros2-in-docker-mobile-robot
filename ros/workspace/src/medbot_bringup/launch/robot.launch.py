import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource

def generate_launch_description():
    pkg_bringup = get_package_share_directory('medbot_bringup')
    robot_bringup_launch = os.path.join(pkg_bringup, 'launch', 'robot_bringup.launch.py')

    return LaunchDescription([
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource(robot_bringup_launch)
        )
    ])
