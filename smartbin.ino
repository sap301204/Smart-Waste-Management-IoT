/*
Filename: firmware/esp32/src/smartbin.ino
Smart Waste Management - ESP32 firmware (Arduino core)
Features: HC-SR04, DHT22, MQ-135, MQTT publish, deep sleep.
*/
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "DHT.h"

#define NODE_ID          "bin-001"
#define WIFI_SSID        "YOUR_SSID"
#define WIFI_PASS        "YOUR_PASSWORD"
#define MQTT_HOST        "broker.hivemq.com"
#define MQTT_PORT        1883
#define BIN_DEPTH_CM     80
#define LEVEL_THRESHOLD  85
#define SLEEP_SECONDS    900
#define SAMPLE_COUNT     5
#define PUBLISH_ON_CHANGE 3
#define USE_DHT22  true
#define DHT_PIN    17
#define DHT_TYPE   DHT22
#define USE_MQ135 false
#define MQ135_PIN  36
#define TRIG_PIN  5
#define ECHO_PIN  18

WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHT_PIN, DHT_TYPE);
float last_published_level = -1;
unsigned long lastMillis = 0;

void setup_wifi() { delay(10); Serial.printf("Connecting to %s\n", WIFI_SSID); WiFi.begin(WIFI_SSID, WIFI_PASS); int tries=0; while (WiFi.status()!=WL_CONNECTED && tries<30){ delay(500); Serial.print("."); tries++; } if (WiFi.status()==WL_CONNECTED){ Serial.println("\\nWiFi connected. IP: "); Serial.println(WiFi.localIP()); } else { Serial.println("\\nWiFi not connected - proceeding (will retry in loop)."); } }

void reconnect_mqtt(){ while(!client.connected()){ Serial.print("Attempting MQTT connection..."); String clientId=String(NODE_ID)+"-"+String(random(0xffff),HEX); if(client.connect(clientId.c_str())){ Serial.println("connected"); } else { Serial.print("failed, rc="); Serial.print(client.state()); Serial.println(" try again in 5 seconds"); delay(5000); } } }

void setup(){ Serial.begin(115200); pinMode(TRIG_PIN, OUTPUT); pinMode(ECHO_PIN, INPUT); dht.begin(); setup_wifi(); client.setServer(MQTT_HOST,MQTT_PORT); lastMillis=millis(); }

float measure_distance_cm(){ long total=0; for(int i=0;i<SAMPLE_COUNT;++i){ digitalWrite(TRIG_PIN,LOW); delayMicroseconds(2); digitalWrite(TRIG_PIN,HIGH); delayMicroseconds(10); digitalWrite(TRIG_PIN,LOW); long duration=pulseIn(ECHO_PIN, HIGH, 30000UL); if(duration==0) duration=30000; float distance_cm=duration*0.0343/2.0; total+=distance_cm; delay(50); } return (float)total/SAMPLE_COUNT; }

float read_mq135(){ if(!USE_MQ135) return -1.0; int raw=analogRead(MQ135_PIN); return (float)raw; }

void publish_payload(float distance_cm,float level_pct,float temp_c,float hum_pct,float gas_raw){ StaticJsonDocument<256> doc; doc["node_id"]=NODE_ID; doc["timestamp"]=millis(); doc["distance_cm"]=round(distance_cm*100.0)/100.0; doc["level_pct"]=round(level_pct*100.0)/100.0; if(USE_DHT22){ doc["temp_c"]=round(temp_c*100.0)/100.0; doc["hum_pct"]=round(hum_pct*100.0)/100.0; } if(USE_MQ135) doc["gas_raw"]=gas_raw; char buf[512]; serializeJson(doc, buf); String topic=String("smartbin/")+NODE_ID+"/data"; client.publish(topic.c_str(), buf); Serial.print("Published to "); Serial.print(topic); Serial.print(": "); Serial.println(buf); if(level_pct>=LEVEL_THRESHOLD){ StaticJsonDocument<96> alert; alert["node_id"]=NODE_ID; alert["level_pct"]=level_pct; alert["msg"]="BIN_FULL"; char abuf[128]; serializeJson(alert, abuf); String atop=String("smartbin/")+NODE_ID+"/alert"; client.publish(atop.c_str(), abuf); Serial.print("Alert published to "); Serial.println(atop); } }

float distance_to_level_pct(float distance_cm){ float clamped=distance_cm; if(clamped<0) clamped=0; if(clamped>BIN_DEPTH_CM) clamped=BIN_DEPTH_CM; float pct=(1.0-(clamped/(float)BIN_DEPTH_CM))*100.0; if(pct<0) pct=0; if(pct>100) pct=100; return pct; }

void loop(){ if(WiFi.status()!=WL_CONNECTED) setup_wifi(); if(!client.connected()) reconnect_mqtt(); client.loop(); float distance=measure_distance_cm(); float level=distance_to_level_pct(distance); float temp=-999, hum=-999, gas=-1; if(USE_DHT22){ temp=dht.readTemperature(); hum=dht.readHumidity(); if(isnan(temp)) temp=-999; if(isnan(hum)) hum=-999; } if(USE_MQ135) gas=read_mq135(); bool should_publish=false; if(last_published_level<0) should_publish=true; if(abs(level-last_published_level)>=PUBLISH_ON_CHANGE) should_publish=true; if(should_publish){ publish_payload(distance, level, temp, hum, gas); last_published_level=level; } else { Serial.printf("Level %.2f%% - change < %.1f -> skip publish\\n", level, (float)PUBLISH_ON_CHANGE); } Serial.printf("Sleeping %d seconds ...\\n", SLEEP_SECONDS); esp_sleep_enable_timer_wakeup((uint64_t)SLEEP_SECONDS * 1000000ULL); delay(100); client.disconnect(); WiFi.disconnect(true); esp_deep_sleep_start(); }
