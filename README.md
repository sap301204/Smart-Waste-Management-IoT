🗑️ Smart Waste Management & Bin Level Detection System (ESP32 + MQTT + Node-RED)

An industry-oriented IoT solution that monitors garbage bin fill levels and sends real-time telemetry and alerts using ESP32, ultrasonic sensing and MQTT.
The project is built to be deployable in real field environments, demo-friendly for college/industries, and interview-ready for engineering roles.

🚀 Project Overview

Traditional garbage collection relies on fixed schedules, leading to:

Overflowing bins in busy zones

Unnecessary pickups in low-usage areas

Increased fuel costs and manpower inefficiency

This project solves it by:

Continuously measuring bin fill level using HC-SR04 ultrasonic sensor

Sending status via MQTT to a cloud broker

Displaying live insights on Node-RED dashboard

Triggering alerts when bin is full

Supporting deep sleep + solar battery setup for long life

🧠 Key Features
Category	Features
Firmware	ESP32, MQTT, JSON telemetry, Deep sleep, DHT22 & MQ-135 optional
Dashboard	Node-RED gauges + charts + alert notifications
Connectivity	MQTT (HiveMQ / Mosquitto / AWS IoT), HTTP fallback
Deployment	IP65 enclosure, solar + battery option, OTA-ready
Demo Assets	200-row dataset, simulator script, screenshots
Documentation	Lab logbook, architecture, interview Q&A, resume bullets
🔧 Hardware Requirements
Component	Quantity
ESP32 Devkit	1
HC-SR04 ultrasonic sensor	1
DHT22 (optional)	1
MQ-135 gas sensor (optional)	1
Power supply	USB / 18650 battery + TP4056 / Solar
Wi-Fi connection	Required for MQTT
📂 Folder Structure
smart-waste-management/
├─ firmware/esp32/              → SmartBin firmware (Arduino)
├─ cloud/node-red/              → Node-RED dashboard flow
├─ dataset/                     → Synthetic dataset (200 rows)
├─ docs/                        → Architecture, Lab logbook, Interview Q&A
├─ dashboard/                   → Dashboard usage guide
├─ simulate_nodes.py            → MQTT simulator (demo without hardware)
├─ demo/                        → Demo steps and screenshot plan
└─ screenshots/                 → Placeholder images for GitHub

⚙️ Quickstart — Run the Project
🔹 1. Flash Firmware to ESP32

Open: firmware/esp32/src/smartbin.ino

Update Wi-Fi name / password + MQTT broker

Upload to ESP32

Open serial monitor → observe telemetry

🔹 2. Set up MQTT Broker

Option A: HiveMQ public broker (no setup required)
Option B: Local Mosquitto

docker run -it -p 1883:1883 eclipse-mosquitto

🔹 3. Import Node-RED Dashboard

Open Node-RED

Menu → Import

Load cloud/node-red/flows.json

You will now see:
✔ Live distance data → level %
✔ Full/threshold alerts
✔ Dashboard gauges + history charts

🧪 Demo Without Hardware (Optional)
python simulate_nodes.py --host localhost --port 1883


✔ Publishes realistic 24-hour smart bin data
✔ Useful for classroom demo / interviews

📊 Important MQTT Topics
Topic	Payload
smartbin/{node_id}/data	Telemetry JSON
smartbin/{node_id}/alert	Alerts when LEVEL% > threshold
Example	smartbin/bin-001/data
🏗 Real-World Deployments (Recommended Setup)

IP65 enclosure

Sensor mounted top-down inside bin

Desiccant + mesh cover to reduce condensation

Solar + 18650 battery supply

Deep sleep sampling every 10–15 mins

OTA firmware update recommended

🎓 Academic & Industry Value

This project satisfies:

Major/Minor engineering project

Internship showcase

Portfolio / resume GitHub project

Industry PoC (Smart City application)

Hackathon project

📌 Screenshots to Upload

Place images inside screenshots/ folder:

Wiring view

MQTT console

Node-RED dashboard gauge (normal)

Node-RED dashboard alert (full)

Chart analytics view

SMS/email alert sample

🧾 License

This project is released under the MIT License — free for academic and commercial use.

🙌 Credits

Developed for Electrical & Electronics / IoT Engineering Students to provide a deployable real-world system that is interview and industry-ready.

⭐ If you use this repository, give it a star on GitHub — it helps others find it!
