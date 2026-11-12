/*****************************************************************************************
 *   ESP32 - programing course
 * 1. example - Toggle LED
 ******************************************************************************************/

const int LED_PIN = 2;
const int BUTTON_PIN = 4;

int ledState = LOW;
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Belső felhúzás: lenyomva LOW, felengedve HIGH
  digitalWrite(LED_PIN, ledState);
}

void loop() {
  int reading = digitalRead(BUTTON_PIN);

  // Állapotváltozás észlelése (időzítő indítása)
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  // Ha a jel stabilabb, mint a prellmentesítési idő
  if ((millis() - lastDebounceTime) > debounceDelay) {
    static int buttonState = HIGH;
    if (reading != buttonState) {
      buttonState = reading;

      // Csak lefutó élre (lenyomás pillanatában) váltunk állapotot
      if (buttonState == LOW) {
        ledState = !ledState;
        digitalWrite(LED_PIN, ledState);
        Serial.println(ledState ? "LED: BE" : "LED: KI");
      }
    }
  }

  lastButtonState = reading;
}