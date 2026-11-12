/*****************************************************************************************
 *   ESP32 - programing course
 * 3. example - Paralell Tasks 
 ******************************************************************************************/

const int LED_PIN = 2;

// Háttérben futó FreeRTOS taszk
void TaskBlink(void *pvParameters) {
  pinMode(LED_PIN, OUTPUT);

  for (;;) {
    digitalWrite(LED_PIN, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS); // Nem blokkolja a másik magot/taszkot
    digitalWrite(LED_PIN, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);

  // Taszk létrehozása és dedikált maghoz rendelése (0-s mag)
  xTaskCreatePinnedToCore(
    TaskBlink,    // Függvény neve
    "BlinkTask",  // Taszk azonosító neve
    2048,         // Veremméret (stack depth byte-ban)
    NULL,         // Paraméter
    1,            // Prioritás (1 = alacsony, magasabb érték = fontosabb)
    NULL,         // Taszk leíró (TaskHandle)
    0             // Mag kiválasztása: Core 0
  );

  Serial.println("FreeRTOS taszk elindult a 0-s magon.");
}

void loop() {
  // A loop() alapértelmezetten az 1-es magon fut
  Serial.print("Fő loop fut a magon: ");
  Serial.println(xPortGetCoreID());
  delay(2000);
}