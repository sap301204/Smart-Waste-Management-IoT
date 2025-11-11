\
// SmartBin_Node.ino
// ESP32 — HC-SR04 + optional DHT11 -> MQTT JSON publisher
// Requirements: PubSubClient, DHT sensor library
#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

// --- CONFIG --- (update before flashing)
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* mqtt_server = "broker.hivemq.com"; // or your mosquitto IP
const int mqtt_port = 1883;
const char* mqtt_topic_data = "smartbin/node1/data";
const char* mqtt_topic_alert = "smartbin/node1/alert";
const char* clientId = "SmartBinNode1";

// --- PINS ---
#define TRIG_PIN 5
#define ECHO_PIN 18
#define DHT_PIN 4
#define DHT_TYPE DHT11  // or DHT22

// --- SENSORS ---
DHT dht(DHT_PIN, DHT_TYPE);

// --- NETWORK ---
WiFiClient wifiClient;
PubSubClient client(wifiClient);

// --- MEASUREMENT ---
long duration_us;
float distance_cm;
const float BIN_DEPTH_CM = 30.0; // measure your bin depth and set accurately

// --- DEBOUNCE / TIMING ---
unsigned long lastPublish = 0;
const unsigned long publishInterval = 15000; // 15s default; set to 60s or more for battery

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  dht.begin();

  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");
  client.setServer(mqtt_server, mqtt_port);
  connectMQTT();
}

void connectMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting MQTT...");
    if (client.connect(clientId)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2s");
      delay(2000);
    }
  }
}

float measureDistanceCM() {
  // HC-SR04 trigger
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration_us = pulseIn(ECHO_PIN, HIGH, 30000); // timeout 30ms
  if (duration_us == 0) return -1.0; // no echo
  distance_cm = (duration_us * 0.0343) / 2.0;
  return distance_cm;
}

void loop() {
  if (!client.connected()) connectMQTT();
  client.loop();

  unsigned long now = millis();
  if (now - lastPublish >= publishInterval) {
    lastPublish = now;

    float dist = measureDistanceCM();
    float level_pct = 0.0;
    if (dist > 0 && dist <= BIN_DEPTH_CM) {
      level_pct = 100.0 - (dist / BIN_DEPTH_CM * 100.0);
      if (level_pct < 0) level_pct = 0;
      if (level_pct > 100) level_pct = 100;
    } else {
      level_pct = 0; // treat as empty or invalid reading
    }

    float temp = NAN, hum = NAN;
    // Read DHT (if available)
    temp = dht.readTemperature();
    hum = dht.readHumidity();
    if (isnan(temp) || isnan(hum)) {
      temp = -999.0;
      hum = -999.0;
    }

    // Prepare JSON payload
    char payload[256];
    snprintf(payload, sizeof(payload),
      "{\"node\":\"node1\",\"level\":%.1f,\"distance_cm\":%.1f,\"temp\":%.1f,\"hum\":%.1f,\"ts\":%lu}",
      level_pct, (dist>0?dist: -1.0), temp, hum, now/1000);

    // Publish data
    client.publish(mqtt_topic_data, payload);
    Serial.print("Published: ");
    Serial.println(payload);

    // Alert rule
    if (level_pct >= 80.0) {
      client.publish(mqtt_topic_alert, "Bin is >80% full");
      // optional local buzzer/LED hook here (digitalWrite(...))
      Serial.println("ALERT published");
    }
  }

  // Sleep mode optimization optional (use deep sleep between publishes)
  // ESP.deepSleep(...) usage omitted for simplicity in this sample.
}
