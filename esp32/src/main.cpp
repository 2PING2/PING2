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

#include <Arduino.h>
#include "config.h"
#include <FastAccelStepper.h>

FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper *stepper = NULL;

#define dirPinStepper    P1_DIR_PIN
#define stepPinStepper   P1_STEP_PIN

void setup() {
    engine.init();
    stepper = engine.stepperConnectToPin(stepPinStepper);
    
    if (stepper) {
        stepper->setDirectionPin(dirPinStepper, P1_INVERT_DIR);
    }
}

void loop() {
    Serial.begin(115200);
    static int speed = 500;       // Vitesse initiale en Hz
    static int acceleration = 100; // Accélération initiale
    static int stepDistance = 300; // Distance du mouvement
    static int increment = 500;    // Incrément de vitesse et d'accélération

    if (stepper) {
        stepper->setSpeedInHz(speed);
        stepper->setAcceleration(acceleration);
        
        // Mouvement aller-retour
        stepper->moveTo(stepDistance, true);
        Serial.println("Moving to " + String(stepDistance) + " steps");
        stepper->moveTo(0, true);
        Serial.println("Moving to 0 steps");
        
        // Augmenter la vitesse et l'accélération progressivement
        speed = 1000;
        acceleration += increment;

        // Attente pour éviter une augmentation trop brutale
    }
    delay(1000);

}
