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
#define EVERYTHING_PUBLIC
#include "LinearActuator.hpp"
// #include <FastAccelStepper.h>

// FastAccelStepperEngine engine = FastAccelStepperEngine();
// FastAccelStepper *stepper = NULL;
LinearActuator la1 = LinearActuator(P1_STEP_PIN, P1_DIR_PIN, TMC1_ADDRESS, P1_INVERT_DIR);


void setup() {
    LinearActuator::setup_all();
    la1.setup();
    Serial.begin(115200);
    if (!la1.motor)
    {
        Serial.println("motor is null");
        while (1)
            ;
    }
        
}

void loop() {
    static int speed = 500;       // Vitesse initiale en Hz
    static int acceleration = 100; // Accélération initiale
    static int stepDistance = 300; // Distance du mouvement
    static int increment = 500;    // Incrément de vitesse et d'accélération

    la1.motor->setSpeedInHz(speed);
    la1.motor->setAcceleration(acceleration);
    
    // Mouvement aller-retour
    la1.motor->moveTo(stepDistance, true);
    Serial.println("Moving to " + String(stepDistance) + " steps");
    la1.motor->moveTo(0, true);
    Serial.println("Moving to 0 steps");
    
    // Augmenter la vitesse et l'accélération progressivement
    speed = 1000;
    acceleration += increment;

    // Attente pour éviter une augmentation trop brutale
    delay(1000);

}
