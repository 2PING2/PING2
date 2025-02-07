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

#include <FastAccelStepper.h>

FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper *stepper = NULL;

#define dirPinStepper    GPIO_NUM_21
#define stepPinStepper   GPIO_NUM_19

void setup() {
   engine.init();
   stepper = engine.stepperConnectToPin(stepPinStepper);
   if (stepper) {
      stepper->setDirectionPin(dirPinStepper);
      stepper->setAutoEnable(true);

      stepper->setSpeedInHz(5000);
      stepper->setAcceleration(1000);
      stepper->moveTo(100*255, true);
      stepper->moveTo(0, true);
   }
}

void loop() {}