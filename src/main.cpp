#include <Arduino.h>

#define LED1 7
#define LED2 8
#define LED3 9
#define LED4 10
#define RLED 6
#define GLED 5
#define BLED 4
#define BTN1 3
#define BTN2 2

int isCharging = 0, lastChargeState = 0, chargingStopped = 0;
int btn1State = 0, lastbtn1State = 0, btn2State = 0, lastbtn2State = 0;
unsigned long lastDebounceTime1 = 0, debounceDelay = 50;
unsigned long btn2PressStart = 0;

void notifyAvailability() {
  delay(1000);
  Serial.println("------------------------------");
  digitalWrite(GLED, HIGH);
  digitalWrite(RLED, LOW);
  Serial.println("Charging station available.");
}

void checkStopCharging() {
  btn2State = digitalRead(BTN2);

  if (btn2State != lastbtn2State) {
    lastDebounceTime1 = millis();
    if (btn2State) btn2PressStart = millis();  
  }

  if (btn2State && (millis() - btn2PressStart >= 3000)) chargingStopped = 1; 

  lastbtn2State = btn2State;
}


void charge() {
  chargingStopped = 0;
  digitalWrite(GLED, LOW);
  digitalWrite(RLED, HIGH);
  
  for (int led = LED1; led <= LED4 && !chargingStopped; led++) {
    Serial.println("Charging...");
    for (int i = 1; i <= 3 && !chargingStopped; i++) {
      digitalWrite(led, HIGH);
      for (int j = 0; j < 600 && !chargingStopped; j += 100) {
        delay(100);  
        checkStopCharging();  
      }
      digitalWrite(led, LOW);
      for (int j = 0; j < 600 && !chargingStopped; j += 100) {
        delay(100);
        checkStopCharging();
      }
    }
    digitalWrite(led, HIGH);
  }

  if (!chargingStopped) {
    Serial.println("Charging completed.");
    for (int i = 0; i < 3 && !chargingStopped; i++) {
      for (int led = LED1; led <= LED4 && !chargingStopped; led++) digitalWrite(led, LOW);
      for (int j = 0; j < 600 && !chargingStopped; j += 100) {
        delay(100);
        checkStopCharging();
      }
      for (int led = LED1; led <= LED4 && !chargingStopped; led++) digitalWrite(led, HIGH);
      for (int j = 0; j < 600 && !chargingStopped; j += 100) {
        delay(100);
        checkStopCharging();
      }
    }
  }

  for (int led = LED1; led <= LED4; led++) digitalWrite(led, LOW);

  if (chargingStopped) Serial.println("Charging stopped.");
  
}


void setup() {
  for (int pin = BLED; pin <= LED4; pin++) pinMode(pin, OUTPUT);
  for (int pin = BTN1; pin <= BTN2; pin++) pinMode(pin, INPUT);

  Serial.begin(9600);

  notifyAvailability();
}

void loop() {
  btn1State = digitalRead(BTN1);
  if (btn1State != lastbtn1State) lastDebounceTime1 = millis();
  if ((millis() - lastDebounceTime1) > debounceDelay) {
    if (btn1State) isCharging = 1;
    else isCharging = 0;
  }
  lastbtn1State = btn1State;

  if (!isCharging && lastChargeState) notifyAvailability();
  else if (isCharging && !lastChargeState) charge();
  lastChargeState = isCharging;
}
