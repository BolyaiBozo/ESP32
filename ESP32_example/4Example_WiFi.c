/*****************************************************************************************
 *   ESP32 - programing course
 * 4. example - WiFi 
 ******************************************************************************************/

#include <WiFi.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

void TaskWiFiKeepAlive(void *pvParameters) {
  WiFi.begin(ssid, password);
  Serial.println("[WiFi Task] Kapcsolódás folyamatban...");

  for (;;) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.print("[WiFi Task] IP cím: ");
      Serial.println(WiFi.localIP());
      Serial.print("[WiFi Task] Jelerősség (RSSI): ");
      Serial.println(WiFi.RSSI());
    } else {
      Serial.println("[WiFi Task] Nincs kapcsolat, újracsatlakozás...");
      WiFi.reconnect();
    }
    // 10 másodpercenként ellenőrizzük az állapotot
    vTaskDelay(10000 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);

  // Wi-Fi kezelő taszk indítása a 0-s magon
  xTaskCreatePinnedToCore(
    TaskWiFiKeepAlive,
    "WiFiTask",
    4096, // A hálózati stacknek érdemes legalább 4 KB vermet adni
    NULL,
    1,
    NULL,
    0
  );
}

void loop() {
  // A helyi vezérlési folyamatok zavartalanul futhatnak az 1-es magon
  Serial.println("[Main Loop] Érzékelők olvasása és vezérlés folyamatban...");
  delay(1000);
}