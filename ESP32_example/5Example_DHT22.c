#include <DHTesp.h>
#include <ArduinoJson.h>

// Lábkiosztás definíciók
#define LED_PIN 2
#define DHT_PIN 15

const unsigned long SEND_INTERVAL = 5000;
unsigned long lastSendTime = 0;

DHTesp dht;

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // DHT22 inicializálása a megadott digitális lábon
  dht.setup(DHT_PIN, DHTesp::DHT22);
}

void loop() {
  // 1. BEJÖVŐ JSON PARANCS KEZELÉSE (LED BE/KI)
  if (Serial.available() > 0) {
    String incoming = Serial.readStringUntil('\n');
    incoming.trim();

    if (incoming.length() > 0) {
      JsonDocument doc;
      DeserializationError error = deserializeJson(doc, incoming);

      if (!error) {
        if (doc["out"].is<const char*>()) {
          String state = doc["out"].as<String>();
          state.toLowerCase();

          if (state == "on") {
            digitalWrite(LED_PIN, HIGH);
          } else if (state == "off") {
            digitalWrite(LED_PIN, LOW);
          }
        }
      } else {
        Serial.print("{\"error\":\"Invalid JSON: ");
        Serial.print(error.c_str());
        Serial.println("\"}");
      }
    }
  }

  // 2. DHT22 ADATOK KÜLDÉSE 5 MÁSODPERCENKÉNT JSON-BAN
  unsigned long currentMillis = millis();
  if (currentMillis - lastSendTime >= SEND_INTERVAL) {
    lastSendTime = currentMillis;

    TempAndHumidity data = dht.getTempAndHumidity();
    JsonDocument outDoc;

    if (!isnan(data.temperature) && !isnan(data.humidity)) {
      outDoc["temperature"] = round(data.temperature * 10.0) / 10.0;
      outDoc["humidity"]    = round(data.humidity * 10.0) / 10.0;
      outDoc["led_state"]   = digitalRead(LED_PIN) ? "on" : "off";
    } else {
      outDoc["error"] = "DHT22 read failed";
    }

    serializeJson(outDoc, Serial);
    Serial.println();
  }