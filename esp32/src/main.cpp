// #define EVERYTHING_PUBLIC
// #include <Arduino.h>
// #include "PING.hpp"

// Player *players[4] = {&PING::player1, &PING::player2, &PING::player3, &PING::player4};

// void setup()
// {
//     Serial.begin(115200);
//     PING::setup();
//     // Serial.println("hello from PING");
// }

// void loop()
// {
//     for (LinearActuator *la : LinearActuator::all)
//         la->run();
// }

#define EVERYTHING_PUBLIC
#include <Arduino.h>
#include "LinearActuator.hpp"

LinearActuator la1(P1_STEP_PIN, P1_DIR_PIN, TMC1_ADDRESS, P1_INVERT_DIR);

void setup()
{
    Serial.begin(115200);
    LinearActuator::setup_all();
    la1.setup();
    Serial.println("hello from LinearActuator");
    la1.move_to(100);
    Serial.println("moved to 100");
    la1.move_to(0);
    Serial.println("moved to 0");
}

void loop()
{
}
