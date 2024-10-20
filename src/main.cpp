#include <Arduino.h>

// Pin assignments
#define LED1 7
#define LED2 8
#define LED3 9
#define LED4 10
#define RLED 6
#define GLED 5
#define BLED 4
#define BTN1 3
#define BTN2 2

// Global variables
int isCharging = 0, lastChargeState = 0, chargingStopped = 0; // charging state trackers
int btn1State = 0, lastbtn1State = 0, btn2State = 0, lastbtn2State = 0; // button state trackers
unsigned long lastDebounceTime1 = 0, debounceDelay = 50, btn2PressStart = 0; // debouncing/delay times

void notifyAvailability() { // indicates that charging is available
  delay(1000);
  Serial.println("------------------------------");
  digitalWrite(GLED, HIGH);
  digitalWrite(RLED, LOW);
  Serial.println("Charging station available.");
}

void checkStopCharging() { // checks if BTN2 (stop button) has been held for 3sec
  btn2State = digitalRead(BTN2);

  if (btn2State != lastbtn2State) { // checks if BTN2 state has changed, resets the debounce timer and tracks press time
    lastDebounceTime1 = millis();
    if (btn2State) btn2PressStart = millis();  
  }

  if (btn2State && (millis() - btn2PressStart >= 3000)) chargingStopped = 1; // flag charging as stopped if BTN2 pressed for 3sec

  lastbtn2State = btn2State; // update state for debouncing logic
}


void charge() { // charging process function
  chargingStopped = 0; // reset stop flag
  digitalWrite(GLED, LOW);
  digitalWrite(RLED, HIGH);
  
  for (int led = LED1; led <= LED4 && !chargingStopped; led++) { // loop through each led
    Serial.println("Charging...");
    for (int i = 1; i <= 3 && !chargingStopped; i++) { // blink 3 times
      digitalWrite(led, HIGH);
      for (int j = 0; j < 600 && !chargingStopped; j += 100) { // micro-delays to check BTN2 state
        delay(100);  
        checkStopCharging();  
      }
      digitalWrite(led, LOW);
      for (int j = 0; j < 600 && !chargingStopped; j += 100) {
        delay(100);
        checkStopCharging();
      }
    }
    digitalWrite(led, HIGH); // leave the LEDs on after completing charge sequence
  }

  if (!chargingStopped) { // if charging is completed without being stopped
    Serial.println("Charging completed.");
    for (int i = 0; i < 3 && !chargingStopped; i++) { // blink all leds 3 times to indicate completion
      for (int led = LED1; led <= LED4 && !chargingStopped; led++) digitalWrite(led, LOW);
      // for (int j = 0; j < 600 && !chargingStopped; j += 100) {
      //   delay(100);
      //   checkStopCharging();
      // }
      delay(600);
      for (int led = LED1; led <= LED4 && !chargingStopped; led++) digitalWrite(led, HIGH);
      // for (int j = 0; j < 600 && !chargingStopped; j += 100) {
      //   delay(100);
      //   checkStopCharging();
      // }
      delay(600);
    }
  }

  for (int led = LED1; led <= LED4; led++) digitalWrite(led, LOW); // turn off all blue leds

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
  // BTN1 debouncing
  if (btn1State != lastbtn1State) lastDebounceTime1 = millis();
  if ((millis() - lastDebounceTime1) > debounceDelay) {
    if (btn1State) isCharging = 1; // if BTN1 pressed - flag charging start
    else isCharging = 0;
  }
  lastbtn1State = btn1State; // update BTN1 state

  if (!isCharging && lastChargeState) notifyAvailability(); // runs if charging stopped/finished (runs once per charge cycle)
  else if (isCharging && !lastChargeState) charge(); // check charging flag and start charging (if not already charging)
  lastChargeState = isCharging; // update charging state
}
