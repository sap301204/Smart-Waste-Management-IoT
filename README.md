# Smart Waste Management & Bin Level Detection System

**Industry-oriented IoT project** to monitor garbage bin fill-level using ESP32 + HC-SR04, publish data via MQTT, visualize in Node-RED and ThingSpeak, and trigger alerts when bins are nearly full.

## Features
- Real-time fill-level measurement (HC-SR04)
- Optional odor (MQ-135) and temperature/humidity (DHT11) sensing
- MQTT JSON publishing (broker.hivemq.com or local Mosquitto)
- Node-RED dashboard + email/SMS alerts
- ThingSpeak time-series analytics (optional)
- Battery + solar deployment recommendations

## Folder structure
(See repo root for full structure)

## Quick Start
1. Clone repo.
2. Edit `code/esp32/SmartBin_Node.ino` credentials: `ssid`, `password`, `mqtt_server`.
3. Install Arduino libs: `PubSubClient`, `DHT sensor library`.
4. Flash to ESP32.
5. Run Node-RED and import `code/node-red/smartbin_nodered_flow.json`.
6. Open Node-RED dashboard (default `http://localhost:1880/ui`) and monitor `Bin Level Gauge`.

## Wiring (short)
- HC-SR04 VCC -> 5V
- HC-SR04 GND -> GND
- TRIG -> GPIO5
- ECHO -> GPIO18 (use voltage divider: 5V -> 3.3V)
- DHT DATA -> GPIO4 (if used)
- MQ-135 analog -> ADC pin (requires calibration)

## Expected Output
- JSON payloads published to `smartbin/node1/data`:
  `{"node":"node1","level":82.0,"distance_cm":5.4,"temp":29.4,"hum":47.1,"ts":1700001200}`

## Troubleshooting
- WiFi fails: check credentials & channel interference.
- Echo returns 0: verify wiring and sensor position (no obstructions).
- Battery drains quickly: increase publish interval, enable deep sleep.

## License
MIT — see `LICENSE_MIT.txt`.

## Credits
Based on common industry best practices and user-supplied brief. :contentReference[oaicite:1]{index=1}

