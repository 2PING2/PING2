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
    Serial.begin(115200);
    Serial.println("Setup done1");
    delay(1000);
    Serial.println("Setup done2");
    delay(1000);
    Serial.println("Setup done3");
    delay(1000);
    Serial.println("Setup done4");
    delay(1000);
    Serial.println("Setup done5");
    delay(1000);

    LinearActuator::setup_all();
    Serial.println("Setup done6");
    la1.setup();
    Serial.println("Setup done7");

    if (!la1.motor)
    {
        while (1)
            {
                Serial.println("motor is null");
                delay(1000);
            }
    }
    Serial.println("Motor setup done");
        
}

void loop() {
    Serial.println("Motor setup done");
    static int speed = 500;       // Vitesse initiale en Hz
    static int acceleration = 100; // Accélération initiale
    static int stepDistance = 300; // Distance du mouvement
    static int increment = 500;    // Incrément de vitesse et d'accélération

    la1.motor->setSpeedInHz(speed);
    la1.motor->setAcceleration(acceleration);
    
    // Mouvement aller-retour
    la1.motor->moveTo(stepDistance);
    delay(1000);
    Serial.println("Moving to " + String(stepDistance) + " steps");
    la1.motor->moveTo(0);
    Serial.println("Moving to 0 steps");
    
    // Augmenter la vitesse et l'accélération progressivement
    speed = 1000;
    acceleration += increment;

    // Attente pour éviter une augmentation trop brutale
    delay(2000);

}
