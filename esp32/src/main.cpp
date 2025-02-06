#define EVERYTHING_PUBLIC
#include <Arduino.h>
#include "PING.hpp"

Player *players[4] = {&PING::player1, &PING::player2, &PING::player3, &PING::player4};

void setup()
{
    Serial.begin(115200);
    PING::setup();
    // Serial.println("hello from PING esp32");
    Serial.println("Calibrating actuators...");
    PING::player3.actuator.calibrate();
    while(!PING::player3.actuator.calibration()){PING::player3.actuator.run();}
    // PING::player2.actuator.calibrate();
    // PING::player3.actuator.calibrate();
    // PING::player4.actuator.calibrate();
    PING::player3.actuator.set_acceleration(LINEAR_ACTUATOR_MAX_ACCELERATION);
    PING::player3.actuator.move_to(100);
    for (int i = 0; i < 10; i++)
    {
    while(PING::player3.actuator.run()){}
    PING::player3.actuator.set_max_speed(50 + PING::player3.actuator.max_speed());
    PING::player3.actuator.move_to(-PING::player3.actuator.current_position());
    }

}

void loop()
{
    for (LinearActuator *la : LinearActuator::all)
            la->run();
}

