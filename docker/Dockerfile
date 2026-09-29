# =============================================================================
# Base Stage: ติดตั้ง Dependencies พื้นฐานและ SDK
# =============================================================================
FROM ros:humble AS base

ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=Asia/Bangkok

# 1. ติดตั้ง Build Tools, Utilities และ Process Management
RUN apt-get update -o Acquire::Check-Valid-Until=false && apt-get install -y --no-install-recommends \
  git \
  cmake \
  pkg-config \
  build-essential \
  libudev-dev \
  psmisc \
  python3-colcon-common-extensions \
  && rm -rf /var/lib/apt/lists/*

# 2. ติดตั้ง ROS 2 Packages (SLAM, Nav2, Foxglove Bridge, Joy, Micro-ROS, Messages & TF)
RUN apt-get update -o Acquire::Check-Valid-Until=false && apt-get install -y --no-install-recommends \
  ros-humble-slam-toolbox \
  ros-humble-navigation2 \
  ros-humble-nav2-bringup \
  ros-humble-nav2-map-server \
  ros-humble-foxglove-bridge \
  ros-humble-joy \
  ros-humble-teleop-twist-joy \
  ros-humble-twist-mux \
  ros-humble-xacro \
  ros-humble-joint-state-publisher \
  ros-humble-robot-state-publisher \
  ros-humble-cyclonedds \
  ros-humble-rmw-cyclonedds-cpp \
  ros-humble-sensor-msgs \
  ros-humble-geometry-msgs \
  ros-humble-laser-geometry \
  ros-humble-rclcpp \
  ros-humble-rclcpp-lifecycle \
  ros-humble-tf2 \
  ros-humble-tf2-ros \
  ros-humble-tf2-tools \
  && rm -rf /var/lib/apt/lists/*

# 3. Clone และ Compile YDLIDAR-SDK
RUN git clone https://github.com/YDLIDAR/YDLidar-SDK.git /tmp/YDLidar-SDK && \
  mkdir -p /tmp/YDLidar-SDK/build && \
  cd /tmp/YDLidar-SDK/build && \
  cmake .. && \
  make -j$(nproc) && \
  make install && \
  rm -rf /tmp/YDLidar-SDK

# 4. Clone และ Compile micro-ROS Agent ไว้ที่ /opt/uros_ws
RUN mkdir -p /opt/uros_ws/src && \
  cd /opt/uros_ws/src && \
  git clone -b humble https://github.com/micro-ROS/micro-ROS-Agent.git && \
  git clone -b humble https://github.com/micro-ROS/micro_ros_msgs.git && \
  cd /opt/uros_ws && \
  . /opt/ros/humble/setup.sh && \
  colcon build && \
  rm -rf /opt/uros_ws/src /opt/uros_ws/build /opt/uros_ws/log

# ตั้งค่า Workspace
RUN mkdir -p /workspace/src
WORKDIR /workspace

# ตั้งค่า ROS 2 Environment
ENV RMW_IMPLEMENTATION=rmw_cyclonedds_cpp
ENV ROS_DOMAIN_ID=0

# Entrypoint Script
COPY docker/entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh
ENTRYPOINT ["/entrypoint.sh"]

# =============================================================================
# Development Stage: สำหรับเขียนโค้ดและพัฒนา (แชร์ Volume ได้)
# =============================================================================
FROM base AS development

RUN apt-get update && apt-get install -y --no-install-recommends \
  nano vim \
  && rm -rf /var/lib/apt/lists/*

RUN echo "source /opt/ros/humble/setup.bash" >> ~/.bashrc
RUN echo "if [ -f /opt/uros_ws/install/setup.bash ]; then source /opt/uros_ws/install/setup.bash; fi" >> ~/.bashrc
RUN echo "if [ -f /workspace/install/setup.bash ]; then source /workspace/install/setup.bash; fi" >> ~/.bashrc

CMD ["bash"]

# =============================================================================
# Production Stage: สำหรับฝังโค้ดและรันอัตโนมัติบนหุ่นยนต์
# =============================================================================
FROM base AS production

COPY ./workspace/src /workspace/src

RUN /bin/bash -c "source /opt/ros/humble/setup.bash && source /opt/uros_ws/install/setup.bash && colcon build --symlink-install"

RUN echo "source /opt/ros/humble/setup.bash" >> ~/.bashrc
RUN echo "source /opt/uros_ws/install/setup.bash" >> ~/.bashrc
RUN echo "source /workspace/install/setup.bash" >> ~/.bashrc

CMD ["ros2", "launch", "medbot_bringup", "robot.launch.py"]