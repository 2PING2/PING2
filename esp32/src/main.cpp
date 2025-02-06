#define EVERYTHING_PUBLIC
#include <Arduino.h>
#include "PING.hpp"

Player *players[4] = {&PING::player1, &PING::player2, &PING::player3, &PING::player4};

void setup()
{
    Serial.begin(115200);
    PING::setup();
    // Serial.println("hello from PING");
}

void loop()
{
    for (LinearActuator *la : LinearActuator::all)
        la->run();
}

