#define EVERYTHING_PUBLIC
#include <Arduino.h>
#include "PING.hpp"

void setup()
{
    Serial.begin(115200);
    PING::setup();
}

void loop()
{
}
