#define EVERYTHING_PUBLIC
#include "PING.hpp"

void setup()
{
    Serial.begin(115200);
    PING::setup();
    Serial.println("Setup done");
    // PING::player1.actuator.moveRight();
    // PING::player1.actuator.setCurrentPosition(0);
}
unsigned long lastTime = 0;
float target = 150;
void loop()
{

    // // PING::player1.actuator.run();
    // if (millis() - lastTime > 100)
    // {
    //     lastTime = millis();
    //     Serial.print(PING::player1.beamSwitch.getState());
    //     Serial.print("\t");
    //     Serial.print(PING::player2.beamSwitch.getState());
    //     Serial.print("\t");
    //     Serial.print(PING::player3.beamSwitch.getState());
    //     Serial.print("\t");
    //     Serial.println(PING::player4.beamSwitch.getState());
    // }
    //     lastTime = millis();
    //     Serial.print(PING::player1.actuator.currentPosition());
    //     Serial.print("\t");
    //     Serial.print(PING::player1.actuator.currentSpeed());
    //     Serial.print("\t");
    //     Serial.print(PING::player1.actuator.currentAcceleration());
    //     Serial.print("\t");
    //     Serial.print(PING::player1.actuator.maxSpeed());
    //     Serial.print("\t");
    //     Serial.print(PING::player1.actuator.driver.getStallGuardResult());
    //     Serial.print("\t");
    //     Serial.println(PING::player1.actuator.driver.isSetupAndCommunicating());

    // }
    // vTaskDelay(2000);
    // Serial.println("Activating solenoids");
    // PING::player1.solenoid.activate();
    // PING::player2.solenoid.activate();
    // PING::player3.solenoid.activate();
    // PING::player4.solenoid.activate();
    // vTaskDelay(1000);
    // Serial.println("Deactivating solenoids");
    // PING::player1.solenoid.deactivate();
    // PING::player2.solenoid.deactivate();
    // PING::player3.solenoid.deactivate();
    // PING::player4.solenoid.deactivate();

}

// // #include <unity.h>
// // #define EVERYTHING_PUBLIC
// // #include "PING.hpp"
// #include "config.h"
// #include <Arduino.h>

// /*
//     mesure du temps d'activation : on allume l'émetteur et on mesure le temps que met le récepteur à détecter le signal
//     mesure du temps de désactivation : on éteint l'émetteur et on mesure le temps que met le récepteur à ne plus détecter le signal

//     protocole de test :
//     - initialisation de l'émetteur avec 'emit' à false et on lance la tâche d'émission avec 'setup_common_emitter'
//     - initialisation du récepteur avec 'setup' et on lance la tâche de réception
//     - on allume l'émetteur en mettant 'emit' à true et en déclenchant le chrono
//     - on arrête le chrono quand le récepteur détecte le signal, avec un timeout de 500ms
//     - on envoie sur le moniteur série le temps mesuré
//     - on éteint l'émetteur en mettant 'emit' à false et en déclenchant le chrono
//     - on arrête le chrono quand le récepteur ne détecte plus le signal, avec un timeout de 500ms
// */

// void setup() 
// {
//     Serial.begin(115200);
//     // pinMode(OUTPUT, 13);
//     // digitalWrite(13, HIGH);
// }

// void loop()
// {
//     delay(100);
// }


// Activation times :9988 9988 9988 9988
// Deactivation times :500000 0 0 0

// /**
//  * Author Teemu Mäntykallio
//  * Initializes the library and runs the stepper
//  * motor in alternating directions.
//  * les connexions
//  *  TMC2209   ESP32
//  * STEP_PIN  GPIO4
//  * TX        GPIO17 (Serial2 TX)
//  * EN        GND
//  */

// #include <TMCStepper.h>

// // #define EN_PIN           38 // Enable
// #define DIR_PIN          55 // Direction
// #define STEP_PIN         GPIO_NUM_4 // Step
// // #define CS_PIN           42 // Chip select
// // #define SW_MOSI          66 // Software Master Out Slave In (MOSI)
// // #define SW_MISO          44 // Software Master In Slave Out (MISO)
// // #define SW_SCK           64 // Software Slave Clock (SCK)
// // #define SW_RX            16 // TMC2208/TMC2224 SoftwareSerial receive pin
// // #define SW_TX            17 // TMC2208/TMC2224 SoftwareSerial transmit pin
// #define SERIAL_PORT Serial2 // TMC2208/TMC2224 HardwareSerial port
// #define DRIVER_ADDRESS 0b00 // TMC2209 Driver address according to MS1 and MS2

// #define R_SENSE 0.11f // Match to your driver
//                       // SilentStepStick series use 0.11
//                       // UltiMachine Einsy and Archim2 boards use 0.2
//                       // Panucatt BSD2660 uses 0.1
//                       // Watterott TMC5160 uses 0.075

// // Select your stepper driver type
// //TMC2130Stepper driver(CS_PIN, R_SENSE);                           // Hardware SPI
// //TMC2130Stepper driver(CS_PIN, R_SENSE, SW_MOSI, SW_MISO, SW_SCK); // Software SPI
// //TMC2660Stepper driver(CS_PIN, R_SENSE);                           // Hardware SPI
// //TMC2660Stepper driver(CS_PIN, R_SENSE, SW_MOSI, SW_MISO, SW_SCK);
// //TMC5160Stepper driver(CS_PIN, R_SENSE);
// //TMC5160Stepper driver(CS_PIN, R_SENSE, SW_MOSI, SW_MISO, SW_SCK);

// // TMC2208Stepper driver(&SERIAL_PORT, R_SENSE);                     // Hardware Serial
// //TMC2208Stepper driver(SW_RX, SW_TX, R_SENSE);                     // Software serial
// TMC2209Stepper driver(&SERIAL_PORT, R_SENSE, DRIVER_ADDRESS);
// //TMC2209Stepper driver(SW_RX, SW_TX, R_SENSE, DRIVER_ADDRESS);

// void setup() {
//     Serial.begin(115200);
// //   pinMode(EN_PIN, OUTPUT);
//   pinMode(STEP_PIN, OUTPUT);
// //   pinMode(DIR_PIN, OUTPUT);
// //   digitalWrite(EN_PIN, LOW);      // Enable driver in hardware

//                                   // Enable one according to your setup
// //SPI.begin();                    // SPI drivers
// SERIAL_PORT.begin(115200);      // HW UART drivers
// // driver.beginSerial(115200);     // SW UART drivers

//   driver.begin();                 //  SPI: Init CS pins and possible SW SPI pins
//                                   // UART: Init SW UART (if selected) with default 115200 baudrate
//   driver.toff(5);                 // Enables driver in software
//   driver.rms_current(1000);        // Set motor RMS current
//   driver.microsteps(16);          // Set microsteps to 1/16th

// // driver.en_pwm_mode(true);       // Toggle stealthChop on TMC2130/2160/5130/5160
// // driver.en_spreadCycle(false);   // Toggle spreadCycle on TMC2208/2209/2224
//   driver.pwm_autoscale(true);     // Needed for stealthChop
// }

// bool shaft = false;

// void loop() {
//   // Run 5000 steps and switch direction in software
//   for (uint16_t i = 1000; i>0; i--) {
//     digitalWrite(STEP_PIN, HIGH);
//     delayMicroseconds(160);
//     digitalWrite(STEP_PIN, LOW);
//     delayMicroseconds(160);
//   }
//   shaft = !shaft;
//   driver.shaft(shaft);
//   Serial.println(shaft);
// }

/**
 * Author Teemu Mäntykallio
 * Initializes the library and runs the stepper
 * motor in alternating directions.
 * les connexions
 *  TMC2209   ESP32
 * STEP_PIN  GPIO4
 * TX        GPIO17 (Serial2 TX)
 * EN        GND
 */

// #include <TMCStepper.h>
// // #include <AccelStepper.h>

// #define STEP_PIN GPIO_NUM_4 // Step
// #define SERIAL_PORT Serial2 // TMC2208/TMC2224 HardwareSerial port
// #define DRIVER_ADDRESS 0b00 // TMC2209 Driver address according to MS1 and MS2

// #define R_SENSE 0.11f

// TMC2209Stepper driver(&SERIAL_PORT, R_SENSE, DRIVER_ADDRESS);
// // AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, GPIO_NUM_5);
// void setup()
// {
//     Serial.begin(115200);
//     pinMode(STEP_PIN, OUTPUT);
//     // SERIAL_PORT.begin(115200);

//     driver.begin();

//     driver.toff(5);
//     driver.pdn_disable(true);
//     driver.rms_current(400);
//     driver.microsteps(16);
//     // driver.pwm_autoscale(false);

//     // driver.pwm_autoscale(true);
//     // driver.shaft(true);
//     // disable cool
//     // driver.COOLCONF(0);

//     // driver.begin();
//     // driver.toff(4);
//     // driver.blank_time(24);
//     // driver.rms_current(400); // mA
//     // driver.microsteps(16);
//     // driver.TCOOLTHRS(0xFFFFF); // 20bit max
//     // driver.semin(5);
//     // driver.semax(2);
//     // driver.sedn(0b01); // set the
//     // driver.SGTHRS(STALL_VALUE);

//     // gconf pdn_uart=1
//     // driver.pdn_disable(1);
//     // stepper.setAcceleration(1000);
//     // stepper.setSpeed(10000);
//     // stepper.enableOutputs();
// }

// bool shaft = false, step_state = false;
// unsigned long t0 = 0, t1 = 0;

// void loop()
// {
//     for (uint16_t i = 1000; i > 0; i--)
//     {
//         digitalWrite(STEP_PIN, HIGH);
//         delayMicroseconds(300);
//         digitalWrite(STEP_PIN, LOW);
//         delayMicroseconds(300);
//     }
//     // stepper.runSpeed();
//     // stepper.run();
//     // if (stepper.distanceToGo() == 0)
//     // {
//     //     stepper.moveTo(1000);
//     // }

//     driver.shaft(shaft);
//     shaft = !shaft;

//     // unsigned long t = esp_timer_get_time();

//     // if (t - t0 > 250)
//     // {
//     //     t0 = t;
//     //     digitalWrite(STEP_PIN, step_state ? HIGH : LOW);
//     //     step_state = !step_state;
//     //     // Serial.println(step_state);
//     // }

//     // if (t - t1 > 500000)
//     // {
//     //     t1 = t;
//     //     driver.shaft(shaft);
//     //     shaft = !shaft;
//     //     Serial.println(driver.cs2rms(driver.cs_actual()), DEC);
//     //     // Serial.println(shaft);
//     // }
// }