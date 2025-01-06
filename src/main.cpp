#include <Arduino.h>

// Pin declarations
const int BUTTON_LEFT_PIN = 3; // Button connected to D3
const int BUTTON_SHOOT_PIN = 4; // Button connected to D4
const int BUTTON_RIGHT_PIN = 5; // Button connected to D5
const int LED_ON_PIN = 6;     // LED connected to D6
const int LED_LEFT_PIN = 7;
const int LED_SHOOT_PIN = 8;
const int LED_RIGHT_PIN = 9;

// Previous button states
bool prevButton1State = HIGH;
bool prevButton2State = HIGH;
bool prevButton3State = HIGH;

// Timing for debounce handling
unsigned long lastDebounceTime1 = 0;
unsigned long lastDebounceTime2 = 0;
unsigned long lastDebounceTime3 = 0;
const unsigned long debounceDelay = 50; // 50 ms for debounce

void setup() {
  // Configure button pins as input with internal pull-up resistors
  pinMode(BUTTON_LEFT_PIN, INPUT_PULLUP);
  pinMode(BUTTON_SHOOT_PIN, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT_PIN, INPUT_PULLUP);

  // Configure LED pins as output
  pinMode(LED_ON_PIN, OUTPUT);
  pinMode(LED_LEFT_PIN, OUTPUT);
  pinMode(LED_SHOOT_PIN, OUTPUT);
  pinMode(LED_RIGHT_PIN, OUTPUT);

  // Ensure LEDs are off at startup
  digitalWrite(LED_ON_PIN, HIGH);
  digitalWrite(LED_LEFT_PIN, HIGH);
  digitalWrite(LED_SHOOT_PIN, HIGH);
  digitalWrite(LED_RIGHT_PIN, HIGH);

  Serial.begin(115200);
}

void loop() {
  // Variables for managing current states and time
  bool button1State = digitalRead(BUTTON_LEFT_PIN) == LOW;
  bool button2State = digitalRead(BUTTON_SHOOT_PIN) == LOW;
  bool button3State = digitalRead(BUTTON_RIGHT_PIN) == LOW;
  unsigned long currentTime = millis();

  // Handle button 1 with debounce
  if (button1State != prevButton1State && (currentTime - lastDebounceTime1) > debounceDelay) {
    lastDebounceTime1 = currentTime;
    prevButton1State = button1State;
    if (button1State) {
      Serial.println("left/push");
      digitalWrite(LED_LEFT_PIN, HIGH);
    } else {
      Serial.println("left/release");
      digitalWrite(LED_LEFT_PIN, LOW);
    }
  }

  // Handle button 2 with debounce
  if (button2State != prevButton2State && (currentTime - lastDebounceTime2) > debounceDelay) {
    lastDebounceTime2 = currentTime;
    prevButton2State = button2State;
    if (button2State) {
      Serial.println("shoot/push");
      digitalWrite(LED_SHOOT_PIN, HIGH);
    } else {
      Serial.println("shoot/release");
      digitalWrite(LED_SHOOT_PIN, LOW);
    }
  }

  // Handle button 3 with debounce
  if (button3State != prevButton3State && (currentTime - lastDebounceTime3) > debounceDelay) {
    lastDebounceTime3 = currentTime;
    prevButton3State = button3State;
    if (button3State) {
      Serial.println("right/push");
      digitalWrite(LED_RIGHT_PIN, HIGH);
    } else {
      Serial.println("right/release");
      digitalWrite(LED_RIGHT_PIN, LOW);
    }
  }
}
