#include "corner.hpp"
#include <Arduino.h>
#include <stdlib.h>
Corner corner;

void setup() {
  Serial.begin(115200);
  Serial.println("Hello World");
  corner.setup();
}

void loop() {
  corner.loop();
  delay(10);
}
