#define EVERYTHING_PUBLIC
#include "PING.hpp"

Player PING::player1 = Player(P1_STEP_PIN, P1_DIR_PIN, TMC1_ADDRESS, P1_INVERT_DIR, P1_SOLENOID_PIN, P1_BEAM_R_PIN);
Player PING::player2 = Player(P2_STEP_PIN, P2_DIR_PIN, TMC2_ADDRESS, P2_INVERT_DIR, P2_SOLENOID_PIN, P2_BEAM_R_PIN);
Player PING::player3 = Player(P3_STEP_PIN, P3_DIR_PIN, TMC3_ADDRESS, P3_INVERT_DIR, P3_SOLENOID_PIN, P3_BEAM_R_PIN);
Player PING::player4 = Player(P4_STEP_PIN, P4_DIR_PIN, TMC4_ADDRESS, P4_INVERT_DIR, P4_SOLENOID_PIN, P4_BEAM_R_PIN);

Player *players[4] = {&PING::player1, &PING::player2, &PING::player3, &PING::player4};


void setup()
{
    Serial.begin(115200);
    PING::setup();
    bool c1 = false, c2 = false, c3 = false, c4 = false;
    while (!(c1 && c2 && c3 && c4))
    {
        int64_t t = esp_timer_get_time();
        PING::player1.actuator.run();
        PING::player2.actuator.run();
        PING::player3.actuator.run();
        PING::player4.actuator.run();
        if(!c1)
        c1 = PING::player1.actuator.calibration(t);
        if(!c2)
        c2 = PING::player2.actuator.calibration(t);
        if(!c3)
        c3 = PING::player3.actuator.calibration(t);
        if(!c4)
        c4 = PING::player4.actuator.calibration(t);
    }

    while (true)
    {
        PING::player1.actuator.run();
        PING::player3.actuator.run();
        PING::player2.actuator.run();
        PING::player4.actuator.run();

        if (PING::player1.actuator.motor.distanceToGo() == 0 && PING::player2.actuator.motor.distanceToGo() == 0 && PING::player3.actuator.motor.distanceToGo() == 0 && PING::player4.actuator.motor.distanceToGo() == 0)
            break;
    }

}

void loop()
{
}
