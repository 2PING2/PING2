#define EVERYTHING_PUBLIC
#include "PING.hpp"

Player *players[4] = {&PING::player1, &PING::player2, &PING::player3, &PING::player4};

void setup()
{
    Serial.begin(115200);
    PING::setup();
    PING::player1.actuator.calibrate();

}
#include <Arduino.h>
void loop()
{
    for (LinearActuator *la : LinearActuator::all)
            la->motor.run();
}
