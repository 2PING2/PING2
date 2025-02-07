#include "Ping.hpp"

TaskHandle_t PING::solenoidOvertempTaskHandle;

RaspComManagement PING::raspComManager = RaspComManagement(RASP_BAUD_RATE);

Player PING::player1 = Player(P1_STEP_PIN, P1_DIR_PIN, TMC1_ADDRESS, P1_INVERT_DIR, P1_SOLENOID_PIN, P1_BEAM_R_PIN);
Player PING::player2 = Player(P2_STEP_PIN, P2_DIR_PIN, TMC2_ADDRESS, P2_INVERT_DIR, P2_SOLENOID_PIN, P2_BEAM_R_PIN);
Player PING::player3 = Player(P3_STEP_PIN, P3_DIR_PIN, TMC3_ADDRESS, P3_INVERT_DIR, P3_SOLENOID_PIN, P3_BEAM_R_PIN);
Player PING::player4 = Player(P4_STEP_PIN, P4_DIR_PIN, TMC4_ADDRESS, P4_INVERT_DIR, P4_SOLENOID_PIN, P4_BEAM_R_PIN);


Vector<Player *> PING::players = Vector<Player *>(4);

void PING::solenoid_overtemp_task(void *pvParameters)
{
    for (;;)
    {
        PING::player1.solenoid.over_temp_protect();
        PING::player2.solenoid.over_temp_protect();
        PING::player3.solenoid.over_temp_protect();
        PING::player4.solenoid.over_temp_protect();
        vTaskDelay(TASK_SOLENOID_OVERTEMP_DELAY_MS / portTICK_PERIOD_MS);
    }
}


void PING::setup()
{
    analogWriteResolution(ANALOG_WRITE_RESOLUTION);
    //BeamSwitch::setup_emitter();
    PING::players[0] = &PING::player1;
    PING::players[1] = &PING::player2;
    PING::players[2] = &PING::player3;
    PING::players[3] = &PING::player4;
    raspComManager.setup(&PING::players);
    LinearActuator::setup_all();
    PING::player1.setup();
    PING::player2.setup();
    PING::player3.setup();
    PING::player4.setup();

    xTaskCreatePinnedToCore(
        PING::solenoid_overtemp_task,         /* Function to implement the task */
        "solenoid_overtemp_task",             /* Name of the task */
        10000,                                /* Stack size in words */
        NULL,                                 /* Task input parameter */
        TASK_SOLENOID_OVERTEMP_PRIORITY,      /* Priority of the task */
        &PING::solenoidOvertempTaskHandle, /* Task handle. */
        TASK_SOLENOID_OVERTEMP_CORE           /* Core where the task should run */
    );

    // Serial.println("beamswitch_1");
    // Serial.println(analogRead(GPIO_NUM_14));
    // Serial.println("beamswitch_2");
    // Serial.println(analogRead(GPIO_NUM_15));
    Serial.print("beamswitch_3 : ");
    Serial.println(analogRead(GPIO_NUM_34));
    // Serial.println("beamswitch_4");
    // Serial.println(analogRead(GPIO_NUM_13));

}