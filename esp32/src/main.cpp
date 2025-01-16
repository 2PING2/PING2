#define EVERYTHING_PUBLIC
#include "PING.hpp"

Player *players[4] = {&PING::player1, &PING::player2, &PING::player3, &PING::player4};

void setup()
{
    Serial.begin(115200);
    // Serial.println("Starting PING");
    PING::setup();
    PING::player1.actuator.calibrate();
    PING::player2.actuator.calibrate();
    PING::player3.actuator.calibrate();
    PING::player4.actuator.calibrate();

}
#include <Arduino.h>
void loop()
{
    for (LinearActuator *la : LinearActuator::all)
            la->motor.run();
}

