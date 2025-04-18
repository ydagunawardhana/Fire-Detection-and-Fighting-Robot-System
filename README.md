# 🚒 ResQBot – Autonomous Fire Response Rover

**ResQBot** is a two-part embedded system designed to detect indoor fires and autonomously dispatch a robotic vehicle to suppress them. It is composed of:

- A **Room Unit** based on an ESP32 that monitors environmental conditions using flame and temperature sensors.
- A **Vehicle Unit** using an Arduino Mega that receives alerts via WiFi and drives a rover with a fire suppression mechanism.

Wireless communication between both units is established using an **ESP-01 WiFi module**, enabling decentralized and responsive fire management.

---

## 🎯 Project Goals

- 🔥 Detect fire in multiple rooms using temperature and flame sensors
- 🚗 Deploy an autonomous vehicle to extinguish fire at the source
- 📡 Enable wireless communication between monitoring and actuation modules
- 🧠 Support decision-making and vehicle control algorithms
- 🛠 Built with **PlatformIO**, allowing easy integration and development

---

## 🧱 System Architecture
```
        ┌────────────────────────────┐        WiFi (ESP-01)        ┌──────────────────────────────┐
        │     ESP32 Fire Monitor     │◄───────────────────────────►│ Arduino Mega + ResQBot Rover │
        │ - Flame & Temp Sensors     │                             │ - Motor Driver & Actuators   │
        │ - Decision Logic           │                             │ - Fire Suppression System    │
        └────────────────────────────┘                             └──────────────────────────────┘
```
---

## 🔌Wiring Diagram


## 🔧 PlatformIO Setup

### 📁 Folder Structure
```
ResQBot/
├── room/                     # ESP32 fire detection system
│   ├── src/
│   ├── lib/
│   ├── include/
│   └── platformio.ini
│
├── vehicle/                  # Arduino Mega fire suppression rover
│   ├── src/
│   ├── lib/
│   ├── include/
│   └── platformio.ini
│
└── README.md
```
---

## 🚀 Getting Started

### 🧰 Prerequisites

- [PlatformIO IDE](https://platformio.org/install) (VS Code recommended)
- USB drivers for ESP32 and Arduino Mega
- Basic knowledge of C++ and microcontroller programming

---

## 🔥 ESP32 Room Monitoring (`room/`)

### 🔨 Initialization

```bash
cd room
pio project init --board esp32dev
```
### ⚙️ platformio.ini
```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200

lib_deps = 
  adafruit/DHT sensor library@^1.4.6
  adafruit/Adafruit Unified Sensor@^1.1.15
  arduino-libraries/ArduinoIoTCloud@^2.4.1
  arduino-libraries/WiFiNINA@^1.9.1
  arduino-libraries/Arduino_ConnectionHandler@^1.0.1

lib_ignore = WiFiNINA
board_build.partitions = no_ota.csv
```
### 📌 Features
	•	Continuously reads data from DHT11/DHT22 and flame sensor
	•	Detects and processes fire events using thresholds
	•	Sends fire alert messages (e.g., ROOM1_FIRE) via ESP-01 WiFi UART
	•	(Optional) Integration with Arduino IoT Cloud

## 🚗 Arduino Mega Vehicle (vehicle/)
## 📦 Initialization
```bash
cd vehicle
pio project init --board megaatmega2560
```
### ⚙️ platformio.ini
```ini
[env:megaatmega2560]
platform = atmelavr
board = megaatmega2560
framework = arduino
monitor_speed = 115200

lib_deps = 
  featherfly/SoftwareSerial@^1.0
  adafruit/Adafruit MPU6050@^2.2.6
  arduino-libraries/Servo@^1.2.2
  bportaluri/WiFiEsp@^2.2.2
```
### 📌 Features
	•	Listens for alerts from ESP-01 via SoftwareSerial
	•	Navigates toward the target location using pre-defined paths
	•	Reads motion data from MPU6050 for stability/orientation
	•	Triggers fire suppression mechanism using a water pump or servo sprayer
## 🔌 Communication Protocol
Simple UART-based string messages are sent from ESP32 to Arduino via ESP-01:

| Message              | Meaning |
| :---------------- | :------: |
| room1       |   Fire detected in Room 1 | 
| room2          |   Fire detected in Room 2   |

---
## 🔩 Hardware Used

|Component | Purpose|
| :---------------- | :------: |
|ESP32 Dev Module | Fire monitoring and logic|
|Arduino Mega 2560 |Vehicle control|
|ESP-01 WiFi Module | WiFi communication bridge| 
|DHT11 |Temperature sensing|
|Flame Sensor |Fire detection|
|L298N Motor Driver| Controls DC motors |
|MPU6050 |Orientation sensor|
|Servo / Water Pump |Fire suppression|
|12V Battery Pack |Power supply for motors and pump|

---

## 🧪 Testing Checklist
	•	✅ ESP32 detects fire and sends correct message
	•	✅ ESP-01 correctly relays message to Arduino
	•	✅ Rover responds to room(x) messages
	•	✅ Suppression system activates after arrival
	•	✅ No false positives or unnecessary dispatch

## 🤝 Contributing

We welcome contributions from the community! Feel free to fork this repo and submit a pull request with your improvements.
	1.	Fork the repo
	2.	Create a branch: git checkout -b feature-name
	3.	Commit your changes
	4.	Push to your fork
	5.	Submit a Pull Request 🚀

## 📬 Contact

For questions or collaboration, feel free to open an issue or email us.

⚠️ Disclaimer: ResQBot is an academic/experimental project and not certified for real-world emergency use.

---

Let me know if you'd like:
- A separate CONTRIBUTING.md file
- Setup video/tutorial links
- Or a `.md` file download link

Happy building and documenting your amazing fire-fighting robot!
