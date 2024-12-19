#include <Arduino.h>

// Déclaration des broches
const int button1Pin = 3; // Bouton connecté à D3
const int button2Pin = 4; // Bouton connecté à D4
const int button3Pin = 5; // Bouton connecté à D5
const int ledPin = 6;     // LED connectée à D6
const int led1Pin = 7;
const int led2Pin = 8;
const int led3Pin = 9;

// États précédents des boutons
bool prevButton1State = HIGH;
bool prevButton2State = HIGH;
bool prevButton3State = HIGH;

// Temps pour gérer l'antirebond
unsigned long lastDebounceTime1 = 0;
unsigned long lastDebounceTime2 = 0;
unsigned long lastDebounceTime3 = 0;
const unsigned long debounceDelay = 50; // 50 ms pour l'antirebond

void setup() {
  // Configuration des broches des boutons en entrée avec pull-up interne
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  pinMode(button3Pin, INPUT_PULLUP);

  // Configuration de la broche de la LED en sortie
  pinMode(ledPin, OUTPUT);
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);

  // Assurez-vous que les LED sont éteintes au démarrage
  digitalWrite(ledPin, HIGH);
  digitalWrite(led1Pin, HIGH);
  digitalWrite(led2Pin, HIGH);
  digitalWrite(led3Pin, HIGH);

  Serial.begin(115200);
}

void loop() {
  // Variables pour gérer les états actuels et le temps
  bool button1State = digitalRead(button1Pin) == LOW;
  bool button2State = digitalRead(button2Pin) == LOW;
  bool button3State = digitalRead(button3Pin) == LOW;
  unsigned long currentTime = millis();

  // Gestion du bouton 1 avec antirebond
  if (button1State != prevButton1State && (currentTime - lastDebounceTime1) > debounceDelay) {
    lastDebounceTime1 = currentTime;
    prevButton1State = button1State;
    if (button1State) {
      Serial.println("left/push");
      digitalWrite(led1Pin, HIGH);
    } else {
      Serial.println("left/release");
      digitalWrite(led1Pin, LOW);
    }
  }

  // Gestion du bouton 2 avec antirebond
  if (button2State != prevButton2State && (currentTime - lastDebounceTime2) > debounceDelay) {
    lastDebounceTime2 = currentTime;
    prevButton2State = button2State;
    if (button2State) {
      Serial.println("shoot/push");
      digitalWrite(led2Pin, HIGH);
    } else {
      Serial.println("shoot/release");
      digitalWrite(led2Pin, LOW);
    }
  }

  // Gestion du bouton 3 avec antirebond
  if (button3State != prevButton3State && (currentTime - lastDebounceTime3) > debounceDelay) {
    lastDebounceTime3 = currentTime;
    prevButton3State = button3State;
    if (button3State) {
      Serial.println("right/push");
      digitalWrite(led3Pin, HIGH);
    } else {
      Serial.println("right/release");
      digitalWrite(led3Pin, LOW);
    }
  }
}
