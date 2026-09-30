# 🛠️ Medbot Robot: Problems & Solutions (Troubleshooting Guide)

เอกสารรวบรวมปัญหาที่เกิดขึ้นในโปรเจกต์ Medbot พร้อมสาเหตุและแนวทางการแก้ไขอย่างละเอียด

---

## 📑 สารบัญ

1. [ปัญหาที่ 1: Docker Build ล้มเหลว (Exit Code: 100) จาก `micro-ros-agent`](#1-docker-build-ล้มเหลว-exit-code-100-จาก-micro-ros-agent)
2. [ปัญหาที่ 2: CycloneDDS โหลดไม่สำเร็จ (`libddsc.so.0: cannot open shared object file`)](#2-cyclonedds-โหลดไม่สำเร็จ-libddscso0-cannot-open-shared-object-file)
3. [ปัญหาที่ 3: ROS 2 Launch ไม่พบ Executable `teleop_twist_joy_node`](#3-ros-2-launch-ไม่พบ-executable-teleop_twist_joy_node)
4. [ปัญหาที่ 4: Launch ไม่พบแพ็กเกจ `ydlidar_ros2_driver`](#4-launch-ไม่พบแพ็กเกจ-ydlidar_ros2_driver)
5. [ปัญหาที่ 5: ไฟล์ในโฟลเดอร์ `ydlidar_ros2_driver` ไม่เข้า GitHub (Gitlink 160000)](#5-ไฟล์ในโฟลเดอร์-ydlidar_ros2_driver-ไม่เข้า-github-gitlink-160000)
6. [ปัญหาที่ 6: YDLIDAR Handshake ล้มเหลว (`Error, cannot retrieve Lidar health code -2`)](#6-ydlidar-handshake-ล้มเหลว-error-cannot-retrieve-lidar-health-code--2)

---

## 1. Docker Build ล้มเหลว (Exit Code: 100) จาก `micro-ros-agent`

### ❌ อาการ (Error)
```text
failed to solve: process "/bin/sh -c apt-get update ... apt-get install -y --no-install-recommends ... ros-humble-micro-ros-agent ..." did not complete successfully: exit code: 100
```

### 🔍 สาเหตุ
ในระบบ ROS 2 Humble แพ็กเกจ **`micro-ros-agent` ไม่มีเป็น binary package บน Ubuntu/ROS APT Repository** ทางการ เมื่อรัน `apt-get install ros-humble-micro-ros-agent` ตัว APT จะหาแพ็กเกจไม่เจอและส่ง exit code 100

### 💡 วิธีแก้ไข
1. ลบ `ros-humble-micro-ros-agent` ออกจากคำสั่ง `apt-get install` ใน `docker/Dockerfile`
2. เพิ่มขั้นตอนการ Clone และ Compile `micro-ROS-Agent` + `micro_ros_msgs` จาก Source code:
```dockerfile
# เพิ่มใน docker/Dockerfile
RUN mkdir -p /opt/uros_ws/src && \
  cd /opt/uros_ws/src && \
  git clone -b humble https://github.com/micro-ROS/micro-ROS-Agent.git && \
  git clone -b humble https://github.com/micro-ROS/micro_ros_msgs.git && \
  cd /opt/uros_ws && \
  . /opt/ros/humble/setup.sh && \
  colcon build && \
  rm -rf /opt/uros_ws/src /opt/uros_ws/build /opt/uros_ws/log
```

---

## 2. CycloneDDS โหลดไม่สำเร็จ (`libddsc.so.0: cannot open shared object file`)

### ❌ อาการ (Error)
```text
[ERROR] [rcl]: Error getting RMW implementation identifier / RMW implementation not installed (expected identifier of 'rmw_cyclonedds_cpp'), with error message 'failed to load shared library 'librmw_cyclonedds_cpp.so' due to dlopen error: libddsc.so.0: cannot open shared object file: No such file or directory...'
medbot-robot exited with code 1 (restarting)
```

### 🔍 สาเหตุ
การใช้คำสั่ง `colcon build --merge-install --install-base /opt/ros/humble` ทำให้ colcon เขียนไฟล์ `setup.bash` ทับไฟล์ระบบเดิมของ `/opt/ros/humble` ส่งผลให้ตัวแปรระบบและ Environment path สำหรับ CycloneDDS (`libddsc.so.0`) สูญหายทั้งหมด

### 💡 วิธีแก้ไข
1. ย้ายการ build micro-ROS workspace ไปไว้ที่โฟลเดอร์ `/opt/uros_ws` แทน (ไม่ทับ `/opt/ros/humble`)
2. เพิ่ม `ros-humble-cyclonedds` ใน `apt-get install`
3. แก้ไข `docker/entrypoint.sh` และ `.bashrc` ให้ Source ตามลำดับ:
```bash
# ใน docker/entrypoint.sh
source /opt/ros/humble/setup.bash

if [ -f /opt/uros_ws/install/setup.bash ]; then
    source /opt/uros_ws/install/setup.bash
fi

if [ -f /workspace/install/setup.bash ]; then
    source /workspace/install/setup.bash
fi

exec "$@"
```

---

## 3. ROS 2 Launch ไม่พบ Executable `teleop_twist_joy_node`

### ❌ อาการ (Error)
```text
[ERROR] [launch]: Caught exception in launch (see debug for traceback): executable 'teleop_twist_joy_node' not found on the libexec directory '/opt/ros/humble/lib/teleop_twist_joy'
medbot-robot exited with code 1 (restarting)
```

### 🔍 สาเหตุ
ในแพ็กเกจ `teleop_twist_joy` ของ ROS 2 ตัวไฟล์ Executable หลักมีชื่อว่า **`teleop_node`** (ไม่ใช่ `teleop_twist_joy_node`)

### 💡 วิธีแก้ไข
แก้ไขในไฟล์ `medbot_bringup/launch/robot_bringup.launch.py` และ `teleop.launch.py`:
```python
teleop_twist_joy_node = Node(
    package='teleop_twist_joy',
    executable='teleop_node',  # แก้ไขจาก teleop_twist_joy_node เป็น teleop_node
    name='teleop_twist_joy_node',
    parameters=[os.path.join(pkg_bringup, 'config', 'joy_params.yaml')],
    remappings=[('/cmd_vel', '/cmd_vel_joy')]
)
```

---

## 4. Launch ไม่พบแพ็กเกจ `ydlidar_ros2_driver`

### ❌ อาการ (Error)
```text
[ERROR] [launch]: Caught exception in launch (see debug for traceback): "package 'ydlidar_ros2_driver' not found, searching: ['/workspace/install/medbot_bringup', '/workspace/install/medbot_description', ...]"
```

### 🔍 สาเหตุ
1. Container รันในโหมด Development ซึ่ง mount Docker Volume `install_cache` ทับ `/workspace/install` โดยที่ตัว Volume ยังไม่เคยผ่านการ `colcon build` แพ็กเกจ `ydlidar_ros2_driver`
2. Container เกิด Crash Loop ทำให้ไม่สามารถรันคำสั่ง `make colcon` แบบ interactive ได้

### 💡 วิธีแก้ไข
1. ปรับ `command` ใน `docker-compose.yml` ให้ auto-build ก่อน launch:
```yaml
command: >
  bash -c "colcon build --symlink-install && source /workspace/install/setup.bash && ros2 launch medbot_bringup robot.launch.py"
```
2. อัปเดต `package.xml` ของ `ydlidar_ros2_driver` เป็น `format="3"`
3. สั่งล้าง Volume แคชเก่าแล้วเริ่มระบบใหม่:
```bash
docker compose down -v
make up
```

---

## 5. ไฟล์ในโฟลเดอร์ `ydlidar_ros2_driver` ไม่เข้า GitHub (Gitlink 160000)

### ❌ อาการ (Issue)
เมื่อเข้าดู Repository บน GitHub พบว่าโฟลเดอร์ `workspace/src/ydlidar_ros2_driver` แสดงเป็นไอคอนโฟลเดอร์สีเทาพร้อมลูกศร และไม่มีไฟล์โค้ดข้างใน

### 🔍 สาเหตุ
เกิดจากตอน Clone แพ็กเกจมาครั้งแรกมีโฟลเดอร์ `.git/` ติดมาด้วย ทำให้ Git หลักบันทึกโฟลเดอร์นี้เป็น **Git Submodule / Gitlink (`mode 160000`)** แทนที่จะ Track ไฟล์ข้างในแบบปกติ

### 💡 วิธีแก้ไข
รันคำสั่งต่อไปนี้ใน Terminal:
```bash
# 1. นำสถานะ Submodule (Gitlink) เก่าออกจาก Git Index (ไฟล์จริงจะไม่หาย)
git rm --cached workspace/src/ydlidar_ros2_driver

# 2. Add โฟลเดอร์และไฟล์ข้างในทั้งหมดกลับเข้ามาใหม่เป็นไฟล์ปกติ
git add workspace/src/ydlidar_ros2_driver

# 3. Commit และ Push ขึ้น GitHub
git commit -m "Track ydlidar_ros2_driver files as normal directory"
git push
```

---

## 6. YDLIDAR Handshake ล้มเหลว (`Error, cannot retrieve Lidar health code -2`)

### ❌ อาการ (Error)
```text
[ydlidar_ros2_driver_node-6] [info] Lidar successfully connected [/dev/ttyUSB1:128000]
[ydlidar_ros2_driver_node-6] [error] Error, cannot retrieve Lidar health code -2
[ydlidar_ros2_driver_node-6] [error] Fail to get baseplate device information!
[ydlidar_ros2_driver_node-6] [error] Failed to start the lidar
```

### 🔍 สาเหตุ & วิธีตรวจสอบ
1. **พารามิเตอร์ DTR และ SingleChannel ในคอนฟิกไม่ถูกต้อง:**
   * สำหรับรุ่น X4/X2: `support_motor_dtr` ต้องเป็น `false` (ถ้าเป็น `true` สัญญาณ DTR จะรบกวน Handshake)
   * `isSingleChannel` ต้องเป็น `true`
2. **การสลับพอร์ต USB (`/dev/ttyUSB0` vs `/dev/ttyUSB1`):**
   * เช็คพอร์ตที่แท้จริงด้วยคำสั่ง:
     ```bash
     ls -l /dev/serial/by-id/
     # หรือ
     dmesg | grep -E "ttyUSB|ttyACM"
     ```
3. **การตั้งค่าพารามิเตอร์ที่ถูกต้องใน `workspace/src/medbot_bringup/config/ydlidar_params.yaml` (สำหรับ X4):**
```yaml
ydlidar_ros2_driver_node:
  ros__parameters:
    port: /dev/ttyUSB1
    frame_id: laser_frame
    ignore_array: ""
    baudrate: 128000
    lidar_type: 1
    device_type: 0
    sample_rate: 5
    abnormal_check_count: 4
    fixed_resolution: true
    reversion: true
    inverted: true
    auto_reconnect: true
    isSingleChannel: true
    intensity: false
    intensity_bit: 0
    support_motor_dtr: false
    angle_max: 180.0
    angle_min: -180.0
    range_max: 10.0
    range_min: 0.12
    frequency: 7.0
```
