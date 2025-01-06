#include <Arduino.h>

// Pin declarations
const int JOYSTICK_PIN = A0; // X-axis of the joystick
const int BUTTON_SHOOT_PIN = 4; // Button pin
const int LED_PIN = 6;     // LED connected to D6

// Previous states
String prevJoystickState = "center";
bool prevButtonState = HIGH;

// For debounce handling
unsigned long lastDebounceTime2 = 0;
bool prevButton2State = HIGH;
const unsigned long debounceDelay = 50; // 50 ms for debounce

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Configure pins as inputs
  pinMode(JOYSTICK_PIN, INPUT);
  pinMode(BUTTON_SHOOT_PIN, INPUT_PULLUP);
  Serial.println("left/release");
  Serial.println("right/release");
}

void loop() {
  // Read the analog value from the joystick X-axis
  int joystickValue = analogRead(JOYSTICK_PIN);
  bool button2State = digitalRead(BUTTON_SHOOT_PIN) == LOW;
  unsigned long currentTime = millis();

  // Determine the current state of the joystick
  String currentJoystickState;
  if (joystickValue < 450) {
    currentJoystickState = "left";
  } 
  else if (joystickValue > 600) 
  {
    currentJoystickState = "right";
  } 
  else {
    currentJoystickState = "center";
  }

  // Detect changes in joystick state
  if (currentJoystickState != prevJoystickState) {
    if (prevJoystickState != "center") {
      Serial.println(prevJoystickState + "/release");
    }
    if (currentJoystickState != "center") {
      Serial.println(currentJoystickState + "/push");
    }
    prevJoystickState = currentJoystickState;
  }

  // Handle button with debounce
  if (button2State != prevButton2State && (currentTime - lastDebounceTime2) > debounceDelay) {
    lastDebounceTime2 = currentTime;
    prevButton2State = button2State;
    if (button2State) {
      Serial.println("shoot/push");
    } else {
      Serial.println("shoot/release");
    }
  }
}
