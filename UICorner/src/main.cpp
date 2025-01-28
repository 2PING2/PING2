#include "corner.hpp"
#include <Arduino.h>
#include <stdlib.h>
Corner corner;

void setup() {
  Serial.begin(9600);
  corner.setup();
}

void loop() {
  corner.loop();
  delay(10);
}
