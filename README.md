📦 Smart Waste Management & Bin Level Detection System

ESP32 • MQTT • Node-RED • Slack/Twilio Alerts • OTA Updates • Deep-Sleep Optimization

🧭 Overview

This project is an industry-ready IoT Smart Waste Management System designed to intelligently monitor garbage bin levels. Using an ESP32, HC-SR04 ultrasonic sensor, and MQTT, the system publishes real-time fill-level data to a cloud dashboard (Node-RED) and sends automated alerts when the bin reaches a critical threshold (>80%).

It includes:

OTA (Over-The-Air) firmware updates

Deep-sleep mode for long-term battery operation

Slack, Twilio (SMS) & Email notifications

Clean GitHub-friendly architecture

This project is deployable in smart cities, housing societies, universities, malls, industrial sites, and more.

🚀 Features
🎛 Hardware Features

ESP32 microcontroller

HC-SR04 level measurement

Optional DHT11/DHT22 temperature & humidity

Optional MQ-135 odor/gas sensing

Battery + solar support

IP65 enclosure support

☁️ IoT & Cloud Features

Real-time MQTT telemetry

Node-RED Dashboard (Gauge + Chart)

Alerts via:

Slack Webhook

Twilio SMS

Email SMTP

🔧 Firmware Features

OTA firmware update support

Deep-sleep logic (default 15 minutes)

JSON-based sensor payload

Lightweight data publishing (PubSubClient)

🏗 System Architecture
         ┌──────────────────┐
         │  Ultrasonic Sensor│
         └──────────┬───────┘
                    │
                    ▼
            ┌─────────────┐
            │    ESP32     │
            │ (WiFi + OTA) │
            └──────┬───────┘
                   │ MQTT Publish
                   ▼
        ┌───────────────────────┐
        │     MQTT Broker        │
        │ (HiveMQ / Mosquitto)  │
        └──────────┬────────────┘
                   │
                   ▼
        ┌───────────────────────┐
        │      Node-RED          │
        │ Dashboard + Alerts     │
        └──────┬──────────┬─────┘
 Slack Webhook │          │ Twilio SMS
               ▼          ▼

🔩 Hardware Requirements
Component	Qty	Description
ESP32	1	Wi-Fi microcontroller
HC-SR04	1	Ultrasonic sensor
DHT11/DHT22	Optional	Environmental sensing
MQ-135	Optional	Gas/odor detection
18650 Battery	Optional	Power backup
TP4056	Optional	Battery charging
Solar Panel	Optional	Outdoor deployment
🔌 Wiring Diagram
Sensor Pin	ESP32 Pin
TRIG	GPIO 5
ECHO	GPIO 18 (use voltage divider 5V → 3.3V)
DHT11 Data	GPIO 4
MQ-135 AO	GPIO 34

Bin depth default = 30 cm (adjust in code).

💻 Software Requirements

Arduino IDE

ESP32 board package

Libraries:

PubSubClient

DHT sensor library

ArduinoOTA

Node-RED + Dashboard nodes

MQTT Broker (HiveMQ / Mosquitto)

📡 MQTT Topics
smartbin/node1/data
smartbin/node1/alert

📊 Sample JSON Payload
{
  "node": "node1",
  "level": 82.4,
  "distance_cm": 5.4,
  "temp": 29.4,
  "hum": 47.1,
  "ts": 1715678890
}

⚙️ Setup Instructions
1️⃣ Flash the ESP32

Open SmartBin_Node_OTA_Sleep.ino

Edit:

WiFi SSID & Password

MQTT broker address

Upload once via USB → after that, OTA works automatically.

2️⃣ Node-RED Setup

Open Node-RED

Import → paste smartbin_flow_slack_twilio.json

Configure:

Slack Webhook URL

Twilio Account SID, Auth Token, and sender number

Email SMTP if needed

Dashboard appears at:

http://localhost:1880/ui

3️⃣ Deep Sleep Logic

Default = 15 minutes
Adjust this line:

const uint64_t SLEEP_SECONDS = 15 * 60;

4️⃣ OTA Update

When device wakes:

It stays awake 10 seconds for OTA

Use Arduino IDE → Port → Network → “SmartBinNode1”

Upload wirelessly 🎉

🧪 Expected Output

Dashboard gauge updates every cycle

Chart shows level trend

Slack/Twilio alerts trigger when level > 80%

MQTT payloads visible via HiveMQ WebSocket client

📁 Repository Structure
/
├─ code/
│  ├─ esp32/
│  │  └─ SmartBin_Node_OTA_Sleep.ino
│  └─ node-red/
│     └─ smartbin_flow_slack_twilio.json
├─ assets/
│  └─ circuit.png
├─ README.md
├─ LICENSE_MIT.txt
└─ .gitignore

📜 License

MIT License — Free to use, modify, and distribute.

🔥 Resume-Ready Project Impact

Developed full-stack IoT system with OTA-capable ESP32 firmware

Implemented real-time monitoring dashboard using Node-RED

Integrated Slack and Twilio for automated IoT alerting

Designed low-power deep-sleep architecture for solar/battery deployment

Completed industry-style documentation + GitHub structure
