#define EVERYTHING_PUBLIC
#include <Arduino.h>
#include "PING.hpp"

void setup()
{
    Serial.begin(115200);
    Serial.println("Setup done1");
    PING::setup();
}

void loop()
{
}
