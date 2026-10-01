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
7. [ปัญหาที่ 7: มอเตอร์หมุนดีเลย์ / ออกตัวไม่พร้อมกัน / มีอาการสะดุด (PID & Feed-Forward)](#7-ปัญหาที่-7-มอเตอร์หมุนดีเลย์--ออกตัวไม่พร้อมกัน--มีอาการสะดุด-pid--feed-forward)
8. [ปัญหาที่ 8: พอร์ต `/dev/ttyUSB*` สลับตำแหน่งกันเมื่อใช้ชิป CP2102 ทั้งคู่ (USB Port Mapping)](#8-ปัญหาที่-8-พอร์ต-devttyusb-สลับตำแหน่งกันเมื่อใช้ชิป-cp2102-ทั้งคู่-usb-port-mapping)
9. [ปัญหาที่ 9: เปลี่ยนมาใช้แบตเตอรี่ 12V แล้วอุปกรณ์ USB หลุด/ไม่ทำงาน (`USB disconnect` & Docker Broken Pipe)](#9-ปัญหาที่-9-เปลี่ยนมาใช้แบตเตอรี่-12v-แล้วอุปกรณ์-usb-หลุดไม่ทำงาน-usb-disconnect--docker-broken-pipe)
10. [ปัญหาที่ 10: ตำแหน่ง LiDAR บน URDF ไม่ตรงกับหุ่นยนต์จริง (URDF Sensor Placement)](#10-ปัญหาที่-10-ตำแหน่ง-lidar-บน-urdf-ไม่ตรงกับหุ่นยนต์จริง-urdf-sensor-placement)
11. [ปัญหาที่ 11: คำสั่ง `map_saver_cli` บันทึกแผนที่ล้มเหลว (`Failed to spin map subscription`)](#11-ปัญหาที่-11-คำสั่ง-map_saver_cli-บันทึกแผนที่ล้มเหลว-failed-to-spin-map-subscription)
12. [ปัญหาที่ 12: SLAM Toolbox Drop ข้อมูลเลเซอร์ทั้งหมด (`timestamp is earlier than transform cache`)](#12-ปัญหาที่-12-slam-toolbox-drop-ข้อมูลเลเซอร์ทั้งหมด-timestamp-is-earlier-than-transform-cache)
13. [ปัญหาที่ 13: สั่งเดินหน้าแล้วหุ่นยนต์หมุนเลี้ยวซ้าย (Motor & Encoder Direction Inversion)](#13-ปัญหาที่-13-สั่งเดินหน้าแล้วหุ่นยนต์หมุนเลี้ยวซ้าย-motor--encoder-direction-inversion)
14. [ปัญหาที่ 14: เดินหน้าแต่ Odometry ถอยหลังทำให้แผนที่ SLAM แตกเป็นแฉก (Inverted Odometry Sign)](#14-ปัญหาที่-14-เดินหน้าแต่-odometry-ถอยหลังทำให้แผนที่-slam-แตกเป็นแฉก-inverted-odometry-sign)
15. [ปัญหาที่ 15: เดินหน้าตรงได้ถูกต้อง แต่เลี้ยวซ้ายแล้วในจอเลี้ยวขวา (Left/Right Encoders Swapped)](#15-ปัญหาที่-15-เดินหน้าตรงได้ถูกต้อง-แต่เลี้ยวซ้ายแล้วในจอเลี้ยวขวา-leftright-encoders-swapped)
16. [ปัญหาที่ 16: จุด LaserScan โผล่อยู่ด้านหลังหุ่นยนต์ 180 องศา (LiDAR Yaw Offset 180°)](#16-ปัญหาที่-16-จุด-laserscan-โผล่อยู่ด้านหลังหุ่นยนต์-180-องศา-lidar-yaw-offset-180)

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

---

## 7. ปัญหาที่ 7: มอเตอร์หมุนดีเลย์ / ออกตัวไม่พร้อมกัน / มีอาการสะดุด (PID & Feed-Forward)

### ❌ อาการ (Symptoms)
1. เมื่อโยกก้าน Joypad มีอาการ Delay (ดีเลย์ 1-2 วินาที) ก่อนที่มอเตอร์จะเริ่มหมุน
2. ล้อซ้ายและขวาออกตัวไม่พร้อมกัน (ข้างหนึ่งเริ่มหมุนก่อน อีกข้างตามมาทีหลัง)
3. ขณะหมุนด้วยความเร็วต่ำ มอเตอร์มีอาการกระตุกหรือสะบัด

### 🔍 สาเหตุ
1. **Derivative Noise Spikes ($K_d > 0$):** การคำนวณ Differential $(\Delta \text{Error} / \Delta t)$ จากสัญญาณ Discrete Encoder ที่ความถี่ 50Hz ทำให้เกิดสัญญาณรบกวน (Noise Spikes) รุนแรง กวนสัญญาณ PWM
2. **แรงเสียดทานสถิตและ Deadband ของชุดเกียร์:** มอเตอร์และชุดทดเกียร์ 60:1 ต้องใช้แรงดัน PWM ขั้นต่ำอย่างน้อย ~30-35 PWM ถึงจะเริ่มชนะแรงเสียดทานสถิต (Static Friction) การใช้ PID ธรรมดาโดยไม่มี Feed-Forward จะต้องรอให้พจน์ Integral ($K_i$) ค่อยๆ สะสมค่า Error จนกว่าจะเกิน Deadband ทำให้เกิดอาการ Lag และข้างที่ฝืดน้อยกว่าจะออกตัวก่อน
3. **Serial Ping Blocking:** การเรียก `rmw_uros_ping_agent` ใน fast loop ของ ESP32 ไปบล็อก Serial I/O ส่งผลให้การรับคำสั่ง `/cmd_vel` ช้าลง

### 💡 วิธีแก้ไข
1. **เพิ่ม Feed-Forward + Deadband Compensation** ใน `firmware/drive_train_test/src/PIDController.cpp`:
```cpp
// Feed-Forward ชดเชยแรงบิดทันทีตาม Target Speed + ชดเชย Deadband 35 PWM
float feedForward = (target / MAX_ROBOT_SPEED_MPS) * (PID_MAX_PWM - MIN_START_PWM);
if (target > 0.005f) {
    feedForward += MIN_START_PWM;
} else if (target < -0.005f) {
    feedForward -= MIN_START_PWM;
}
```
2. **ปรับจูนค่า PID ใน `firmware/drive_train_test/include/Config.h`:**
   * ตั้งค่า $K_d = 0.0$ เพื่อตัดสัญญาณกระตุก
   * ตั้งค่า $K_p = 80.0, K_i = 10.0$
   * กำหนดเพดานความปลอดภัย `PID_MAX_PWM = 200` และ `PID_MIN_PWM = -200`
3. **ลดความถี่ Ping Agent ใน `main.cpp`** ให้เช็คทุกๆ 5 วินาทีด้วย Timeout 10ms เพื่อไม่ให้บล็อก Loop ควบคุม

---

## 8. ปัญหาที่ 8: พอร์ต `/dev/ttyUSB*` สลับตำแหน่งกันเมื่อใช้ชิป CP2102 ทั้งคู่ (USB Port Mapping)

### ❌ อาการ (Symptoms)
เมื่อเสียบสาย USB หรือเปิดเครื่องใหม่ บางครั้ง LiDAR ทำงานแต่ ESP32 ไม่เชื่อมต่อ หรือ micro-ROS ต่อไปที่พอร์ตของ LiDAR ทำให้ทั้งสองระบบ Error

### 🔍 สาเหตุ
ทั้งบอร์ด ESP32 (NodeMCU/DevKit) และโมดูลแปลงสัญญาณของ YDLIDAR X4 ใช้ชิปแปลง USB-to-UART ยี่ห้อ **Silicon Labs CP2102** เหมือนกัน:
* `idVendor=10c4, idProduct=ea60`
* `SerialNumber=0001` เหมือนกันทั้งสองตัว
ทำให้ Linux และ udev ไม่สามารถแยกแยะผ่าน `/dev/serial/by-id/` ได้ และกำหนดชื่อ `ttyUSB0` หรือ `ttyUSB1` ตามลำดับที่ระบบเสียบ/ตรวจพบก่อนหลัง

### 💡 วิธีแก้ไข & การแยกพอร์ต
1. **แยกตามตำแหน่งพอร์ตทางกายภาพ (Physical USB Path):**
   ดู path ถาวรด้วยคำสั่ง:
   ```bash
   ls -l /dev/serial/by-path/
   ```
   * ตัวอย่าง: `platform-fd500000.pcie-pci-0000:01:00.0-usb-0:1.3:1.0-port0` (ช่อง USB ด้านบน/ล่างของบอร์ด Pi)
2. **สร้าง Udev Rules ถาวร (Symlink `/dev/ttyLIDAR` และ `/dev/ttyESP32`):**
   สร้างไฟล์ `/etc/udev/rules.d/99-medbot-serial.rules`:
   ```bash
   # แมปตามช่อง USB (KERNELS)
   SUBSYSTEM=="tty", KERNELS=="1-1.1:1.0", SYMLINK+="ttyESP32", MODE="0666"
   SUBSYSTEM=="tty", KERNELS=="1-1.3:1.0", SYMLINK+="ttyLIDAR", MODE="0666"
   ```
   แล้วสั่งโหลด rule ใหม่:
   ```bash
   sudo udevadm control --reload-rules && sudo udevadm trigger
   ```

---

## 9. ปัญหาที่ 9: เปลี่ยนมาใช้แบตเตอรี่ 12V แล้วอุปกรณ์ USB หลุด/ไม่ทำงาน (`USB disconnect` & Docker Broken Pipe)

### ❌ อาการ (Symptoms)
เมื่อต่อไฟเลี้ยงจากแบตเตอรี่ 12V Li-ion (50Ah) เข้าระบบ Raspberry Pi พบว่า:
1. ใน `dmesg` ขึ้นแจ้งเตือน:
   ```text
   usb 1-1.1: USB disconnect, device number ...
   cp210x ttyUSB0: cp210x converter now disconnected
   ```
2. ใน Docker รัน `ros2 topic list` แล้วไม่มี `/scan` และ `/odom` ขึ้นมา
3. สั่งรันคำสั่งบนพอร์ตเดิมแล้วขึ้น `SerialException` หรือ Broken pipe

### 🔍 สาเหตุ
1. **USB Rail Voltage Droop (ไฟ 5V VBUS ดรอปชั่วขณะ):** บอร์ดแปลง Step-down (12V $\rightarrow$ 5V) จ่ายกระแส (Amp) ไม่พอ หรือสายไฟ 5V เส้นเล็ก เมื่อ LiDAR หมุนมอเตอร์และ ESP32 ดึงกระแสพร้อมกัน รางไฟพอร์ต USB ของ Pi จะดรอปต่ำกว่า 4.7V ทำให้ชิป CP2102 ดับและรีเซ็ตตัวเอง
2. **ไฟ 5V สองแหล่งชนกัน (Backfeeding):** หาก ESP32 มีการรับไฟ 5V มาจากบอร์ดไดรฟ์มอเตอร์ (BTS7960) หรือ Step-Down ด้วย และยังเสียบสาย USB เข้า Pi ไฟ 5V จะวิ่งย้อนเข้าพอร์ต USB จนระบบตัดไฟป้องกัน
3. **Docker Process Broken Pipe:** เมื่อพอร์ต USB รีเซ็ตในระดับ Linux โหนดที่กำลังรันอยู่ใน Docker Container จะสูญเสีย File Descriptor เดิมไป ทำให้ไม่สามารถอ่าน/เขียนข้อมูลได้อีกแม้ USB จะต่อกลับมาแล้ว

### 💡 วิธีแก้ไข
1. **เลือกใช้ Buck Converter 12V $\rightarrow$ 5V ขนาด 5A ขึ้นไป** และใช้สายไฟเส้นใหญ่ (18-20 AWG) เพื่อป้องกัน Voltage Drop
2. **แยกสายไฟเลี้ยงให้ถูกต้อง:**
   * ให้ ESP32 รับไฟเลี้ยงจากสาย USB ของ Pi เพียงทางเดียว (ไม่ต้องต่อไฟ 5V ภายนอกเข้าขา VIN ซ้ำซ้อน)
   * กราวด์ (GND) ทุกระบบ (แบตเตอรี่, ไดรฟ์มอเตอร์, ESP32, Pi) ต้องเชื่อมต่อถึงกันทั้งหมด (Single Common Ground)
3. **สั่งรีสตาร์ท Container เมื่อมีการเสียบสาย USB ใหม่:**
   ```bash
   docker compose restart robot
   ```
   หรือ
   ```bash
   docker compose down && docker compose up -d robot
   ```
4. **ตรวจสอบสถานะอุปกรณ์หลังต่อใหม่:**
   ```bash
   lsusb
   ls -l /dev/ttyUSB*
   docker compose logs -f robot
   ```

---

## 10. ปัญหาที่ 10: ตำแหน่ง LiDAR บน URDF ไม่ตรงกับหุ่นยนต์จริง (URDF Sensor Placement)

### ❌ อาการ (Symptoms)
เมื่อเปิดดูหุ่นยนต์ใน Foxglove Studio / RViz2 พบว่าโมเดล LiDAR (ทรงกระบอกสีดำ `laser_frame`) ลอยเยื้องไปข้างหน้า ไม่ได้อยู่ตรงกึ่งกลางของตัวหุ่น (`base_link`)

### 🔍 สาเหตุ
ในไฟล์ `ros/workspace/src/medbot_description/urdf/medbot.urdf.xacro` มีการตั้งค่า `laser_joint` ให้เยื้องแกน X ไปข้างหน้า `0.15` เมตร (`origin xyz="0.15 0.0 0.12"`)

### 💡 วิธีแก้ไข
ปรับตำแหน่งแกน `xyz` ใน `medbot.urdf.xacro` ให้เป็นกึ่งกลาง `0.0 0.0 0.10`:
```xml
  <joint name="laser_joint" type="fixed">
    <parent link="base_link"/>
    <child link="laser_frame"/>
    <!-- Position LiDAR in the center of the robot chassis -->
    <origin xyz="0.0 0.0 0.10" rpy="0 0 0"/>
  </joint>
```

---

## 11. ปัญหาที่ 11: คำสั่ง `map_saver_cli` บันทึกแผนที่ล้มเหลว (`Failed to spin map subscription`)

### ❌ อาการ (Error)
```text
[INFO] [map_saver]: Saving map from 'map' topic to '/workspace/src/medbot_bringup/maps/my_map' file
[WARN] [map_saver]: Free threshold unspecified. Setting it to default value: 0.250000
[WARN] [map_saver]: Occupied threshold unspecified. Setting it to default value: 0.650000
[ERROR] [map_saver]: Failed to spin map subscription
[INFO] [map_saver]: Destroying
[ros2run]: Process exited with failure 1
```

### 🔍 สาเหตุ
คำสั่ง `map_saver_cli` รอรับข้อมูลจาก Topic `/map` เกิน Timeout ค่าเริ่มต้น (2 วินาที) โดยที่ `slam_toolbox` ยังไม่ได้ Publish ข้อมูลแผนที่รอบใหม่ออกมา หรือโหมด SLAM ยังไม่ได้เริ่มทำงาน

### 💡 วิธีแก้ไข
1. **บันทึกผ่าน Service ของ SLAM Toolbox โดยตรง (แนะนำที่สุด):**
   ```bash
   ros2 service call /slam_toolbox/save_map slam_toolbox/srv/SaveMap "{name: {data: '/workspace/src/medbot_bringup/maps/my_map'}}"
   ```
2. **หรือขยายเวลา Timeout ให้กับ `map_saver_cli` เป็น 10 วินาที:**
   ```bash
   ros2 run nav2_map_server map_saver_cli -f /workspace/src/medbot_bringup/maps/my_map --timeout 10000
   ```

---

## 12. ปัญหาที่ 12: SLAM Toolbox Drop ข้อมูลเลเซอร์ทั้งหมด (`timestamp is earlier than transform cache`)

### ❌ อาการ (Error)
เมื่อสั่งรัน `ros2 launch medbot_bringup slam.launch.py` ตัวโหนด `sync_slam_toolbox_node` แจ้งเตือนข้อความซ้ำๆ และไม่สามารถวาดแผนที่ได้:
```text
[sync_slam_toolbox_node-1] [INFO] [slam_toolbox]: Message Filter dropping message: frame 'laser_frame' at time ... for reason 'the timestamp on the message is earlier than all the data in the transform cache'
```

### 🔍 สาเหตุ
1. **Clock Skew / Latency จาก SDK ของ LiDAR:** ไดรเวอร์ `ydlidar_ros2_driver` ใช้ค่า Timestamp ภายใน SDK (`scan.stamp`) ซึ่งมี Latency จากบัฟเฟอร์ Serial ทำให้อายุของข้อความ LaserScan ช้ากว่าเวลาใน TF Tree ของระบบ ROS 2 ปัจจุบัน (~0.5 วินาที) ตัว TF Message Filter จึงประเมินว่าเป็นข้อมูลเก่าและปฏิเสธทั้งหมด
2. **ขาดการ Broadcast TF `odom -> base_footprint`:** บอร์ด ESP32 ส่งเฉพาะข้อความ `nav_msgs/msg/Odometry` บน Topic `/odom` แต่ยังไม่มีโหนดเชื่อมแปลงข้อมูล Odometry เข้าสู่ TF Tree

### 💡 วิธีแก้ไข
1. **แก้ไข `ydlidar_ros2_driver_node.cpp` ให้ใช้เวลา ROS ปัจจุบัน (`node->now()`):**
   ```cpp
   scan_msg->header.stamp = node->now();
   scan_msg->header.frame_id = frame_id;
   pc_msg->header = scan_msg->header;
   ```
2. **สร้างโหนด `odom_to_tf.py` ใน `medbot_bringup` เพื่อ Broadcast TF อัตโนมัติ:**
   * Subscribe `/odom` แล้ว Broadcast Transform `odom -> base_footprint`
   * เพิ่มเข้าไปใน `robot_bringup.launch.py`
3. **ปรับเพิ่ม Timeout ใน `mapper_params_online_sync.yaml`:**
   ```yaml
   transform_timeout: 0.5  # ปรับเพิ่มจาก 0.2 เป็น 0.5
   ```

---

## 13. ปัญหาที่ 13: สั่งเดินหน้าแล้วหุ่นยนต์หมุนเลี้ยวซ้าย (Motor & Encoder Direction Inversion)

### ❌ อาการ (Symptoms)
เมื่อดันก้าน Joystick เดินหน้าตรง (`linear.x > 0, angular.z = 0`) ตัวหุ่นยนต์บนพื้นจริงหมุนเลี้ยวซ้ายรอบตัวเองแทนที่จะวิ่งตรงไปข้างหน้า

### 🔍 สาเหตุ
มอเตอร์ฝั่งซ้าย (Left Motor) ต่อสายสัญญาณ PWM หรือขั้วมอเตอร์กลับทิศ ทำให้เมื่อได้รับคำสั่งเดินหน้า มอเตอร์ขวาหมุนไปข้างหน้า แต่มอเตอร์ซ้ายหมุนถอยหลัง ส่งผลให้หุ่นหมุนควงซ้าย

### 💡 วิธีแก้ไข
สลับคู่พิน PWM และคู่พิน Encoder ของล้อซ้ายใน `firmware/drive_train_test/include/Config.h`:
```cpp
// Left Motor - BTS7960 Driver Pins
#define LEFT_MOTOR_RPWM       26  // สลับจาก 25 เป็น 26
#define LEFT_MOTOR_LPWM       25  // สลับจาก 26 เป็น 25

// Left Encoder Pins
#define LEFT_ENCODER_A        22  // สลับจาก 23 เป็น 22 เพื่อให้นับทิศทางถูกต้อง
#define LEFT_ENCODER_B        23  // สลับจาก 22 เป็น 23
```
*หมายเหตุ: จำเป็นต้องสลับพิน Encoder ควบคู่ไปด้วย เพื่อให้ลูป PID Controller วัดทิศทางการหมุนของล้อซ้ายได้ถูกต้อง*

---

## 14. ปัญหาที่ 14: เดินหน้าแต่ Odometry ถอยหลังทำให้แผนที่ SLAM แตกเป็นแฉก (Inverted Odometry Sign)

### ❌ อาการ (Symptoms)
เมื่อเดินหุ่นยนต์ตรงไปในทางเดิน แผนที่ใน Foxglove Studio วาดกำแพงซ้อนทับกันเป็นแฉกๆ (Starburst / Fan pattern) และตัวหุ่นยนต์บนหน้าจอแสดงผลเคลื่อนที่ถอยหลังทั้งที่หุ่นยนต์จริงเดินหน้า

### 🔍 สาเหตุ
ทิศทางการนับของ Encoder (A/B State Transitions) นับค่าลดลง (ติดลบ) เมื่อล้อหมุนเดินหน้า ทำให้ `deltaTicks < 0` $\rightarrow$ ข้อมูล `/odom` ส่งค่า $\Delta x < 0$ (ถอยหลัง) ขัดแย้งกับข้อมูลระยะทางที่ลดลงจริงจาก LiDAR ระบบ SLAM Toolbox จึงประเมินการเคลื่อนที่ผิดพลาดอย่างรุนแรงและหมุนสลับระนาบแผนที่ไปมา

### 💡 วิธีแก้ไข
1. **สลับพิน Encoder ทั้งสองข้างใน `firmware/drive_train_test/include/Config.h`:**
   ```cpp
   // Left Encoder Pins
   #define LEFT_ENCODER_A        23  // สลับเพื่อให้นับบวกเมื่อเดินหน้า
   #define LEFT_ENCODER_B        22

   // Right Encoder Pins
   #define RIGHT_ENCODER_A       18  // สลับเพื่อให้นับบวกเมื่อเดินหน้า
   #define RIGHT_ENCODER_B       19
   ```
2. **ตรวจสอบระยะ Track Width และจุดกึ่งกลางของล้อ:**
   * `TRACK_WIDTH_M` = `0.30f` (300 mm)
   * จุดยึด `base_link` และ `laser_frame` ใน `medbot.urdf.xacro` อยู่ที่กึ่งกลาง `(X=0.0, Y=0.0)`
3. **Flash ESP32 ใหม่** แล้วทดสอบเข็นตรง 1 เมตร ค่า `x` ใน `/odom` ต้องเพิ่มขึ้นเป็นบวก

---

## 15. ปัญหาที่ 15: เดินหน้าตรงได้ถูกต้อง แต่เลี้ยวซ้ายแล้วในจอเลี้ยวขวา (Left/Right Encoders Swapped)

### ❌ อาการ (Symptoms)
เมื่อขับหุ่นยนต์เดินหน้าตรง ค่าพิกัด `x` ใน `/odom` เพิ่มขึ้นเป็นบวกถูกต้อง แต่เมื่อเลี้ยวซ้าย (ทวนเข็มนาฬิกา) ค่า `twist.twist.angular.z` ติดลบ (`-`) และโมเดลหุ่นยนต์ใน Foxglove กลับหมุนเลี้ยวขวา

### 🔍 สาเหตุ
สายสัญญาณ Encoder ล้อซ้ายและล้อขวาสลับข้างกันในระดับพิน GPIO:
* เมื่อหมุนเลี้ยวซ้าย: ล้อขวาหมุนไปข้างหน้า (+), ล้อซ้ายหมุนถอยหลัง (-)
* แต่เนื่องจากพินสลับข้างกัน: ซอฟต์แวร์เข้าใจว่าล้อซ้ายหมุนหน้า และล้อขวาหมุนหลัง ทำให้คำนวณ $\Delta\theta = (Right - Left) / W$ ได้ค่าติดลบ (เลี้ยวขวา)

### 💡 วิธีแก้ไข
สลับคู่พินของ Left Encoder และ Right Encoder ใน `firmware/drive_train_test/include/Config.h`:
```cpp
// Left Encoder Pins
#define LEFT_ENCODER_A        18  // สลับคู่ล้อซ้าย-ขวา เพื่อให้หมุนซ้ายเป็นบวก (+)
#define LEFT_ENCODER_B        19

// Right Encoder Pins
#define RIGHT_ENCODER_A       23  // สลับคู่ล้อซ้าย-ขวา เพื่อให้หมุนซ้ายเป็นบวก (+)
#define RIGHT_ENCODER_B       22
```

---

## 16. ปัญหาที่ 16: จุด LaserScan โผล่อยู่ด้านหลังหุ่นยนต์ 180 องศา (LiDAR Yaw Offset 180°)

### ❌ อาการ (Symptoms)
เมื่อดูในหน้าต่าง 3D ของ Foxglove Studio พบว่าจุดเลเซอร์และกำแพงห้องที่อยู่ด้านหน้าหุ่นยนต์จริง ไปปรากฏอยู่ด้านหลังกล่องหุ่นยนต์สีน้ำเงิน (กลับทิศ 180 องศา)

### 🔍 สาเหตุ
ตำแหน่งการติดตั้งตัว LiDAR หรือมุมอ้างอิงศูนย์องศา (Zero Index) ของไดรเวอร์ YDLIDAR หมุนกลับหลัง 180 องศาเทียบกับพิกัด `base_link` ของตัวรถ

### 💡 วิธีแก้ไข
หมุนระนาบ `laser_joint` ใน `ros/workspace/src/medbot_description/urdf/medbot.urdf.xacro` ตามแกน Z ไป 180 องศา (`pi = 3.1415926` เรเดียน):
```xml
  <joint name="laser_joint" type="fixed">
    <parent link="base_link"/>
    <child link="laser_frame"/>
    <!-- Rotate 180 deg (pi) to align 0-deg scan with robot front -->
    <origin xyz="0.0 0.0 0.10" rpy="0 0 3.1415926"/>
  </joint>
```
และสั่ง `docker compose restart robot` เพื่ออัปเดตโมเดล TF ใน ROS 2






