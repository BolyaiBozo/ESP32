/*****************************************************************************************
 *   ESP32 - programing course
 * 2. example - External Interrupt
 ******************************************************************************************/

const int LED_PIN = 2;
const int BUTTON_PIN = 4;

volatile bool buttonPressed = false;
volatile unsigned long lastInterruptTime = 0;

// Megszakítási rutin - gyorsnak és minimálisnak kell lennie
void IRAM_ATTR isrButton() {
  unsigned long currentTime = millis();
  // 200 ms szoftveres prellmentesítés a megszakításon belül
  if (currentTime - lastInterruptTime > 200) {
    buttonPressed = true;
    lastInterruptTime = currentTime;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Megszakítás hozzárendelése lefutó élre (FALLING)
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), isrButton, FALLING);
}

void loop() {
  if (buttonPressed) {
    buttonPressed = false;
    static bool state = false;
    state = !state;
    digitalWrite(LED_PIN, state ? HIGH : LOW);
    Serial.println("Megszakítás feldolgozva: állapot váltva.");
  }
  
  // A processzor itt más feladatokat végezhet blokkolás nélkül
}