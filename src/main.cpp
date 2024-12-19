#include <Arduino.h>

// Déclaration des broches
const int JoystickPin = A0; // Axe X du joystick
const int Bouton2Pin = 4; // Pin du bouton
const int ledPin = 6;     // LED connectée à D6

// Etats précédents
String prevJoystickState = "center";
bool prevButtonState = HIGH;

// pour l'anti rebond 
unsigned long lastDebounceTime2 = 0;
bool prevButton2State = HIGH;
const unsigned long debounceDelay = 50; // 50 ms pour l'antirebond

void setup() {
  // Initialiser la communication série
  Serial.begin(115200);

  // Configurer les broches comme entrées
  pinMode(JoystickPin, INPUT);
  pinMode(Bouton2Pin, INPUT_PULLUP);
  Serial.println("left/release");
  Serial.println("right/release");

}

void loop() {
  // Lire la valeur analogique de l'axe X du joystick
  int JoystickValue = analogRead(JoystickPin);
  bool button2State = digitalRead(Bouton2Pin) == LOW;
  unsigned long currentTime = millis();

  // Déterminer l'état actuel du joystick
  String currentJoystickState;
  if (JoystickValue < 450) {
    currentJoystickState = "left";
  } 
  else if (JoystickValue > 600) 
  {
    currentJoystickState = "right";
  } 
  else {
    currentJoystickState = "center";
  }

  // Détection de changement pour le joystick
  if (currentJoystickState != prevJoystickState) {
    if (prevJoystickState != "center") {
      Serial.println(prevJoystickState + "/release");
    }
    if (currentJoystickState != "center") {
      Serial.println(currentJoystickState + "/push");
    }
    prevJoystickState = currentJoystickState;
  }

  // pour le bouton
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
