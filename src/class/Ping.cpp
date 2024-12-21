#include "Ping.hpp"

TaskHandle_t PING::run_task_handle;

Player PING::player1 = Player(P1_STEP_PIN, P1_DIR_PIN, TMC1_ADDRESS, P1_SOLENOID_PIN, P1_BEAM_R_PIN);
Player PING::player2 = Player(P2_STEP_PIN, P2_DIR_PIN, TMC2_ADDRESS, P2_SOLENOID_PIN, P2_BEAM_R_PIN);
Player PING::player3 = Player(P3_STEP_PIN, P3_DIR_PIN, TMC3_ADDRESS, P3_SOLENOID_PIN, P3_BEAM_R_PIN);
Player PING::player4 = Player(P4_STEP_PIN, P4_DIR_PIN, TMC4_ADDRESS, P4_SOLENOID_PIN, P4_BEAM_R_PIN);

RaspComManagement PING::raspComManager = RaspComManagement(RASP_TX_PIN, RASP_RX_PIN, RASP_BAUD_RATE);

void PING::run_task(void *pvParameters)
{
    for (;;)
    {
        uint64_t currentTime = esp_timer_get_time();
        // PING::player1.beamSwitch.check(currentTime);
        // PING::player2.beamSwitch.check(currentTime);
        // PING::player3.beamSwitch.check(currentTime);
        // PING::player4.beamSwitch.check(currentTime);
        PING::player1.solenoid.overTempProtect(currentTime);
        PING::player2.solenoid.overTempProtect(currentTime);
        PING::player3.solenoid.overTempProtect(currentTime);
        PING::player4.solenoid.overTempProtect(currentTime);
        vTaskDelay(TASK_BEAM_EMITTER_DELAY_MS / portTICK_PERIOD_MS);
    }
}
void PING::setup()
{
    analogWriteResolution(ANALOG_WRITE_RESOLUTION);
    BeamSwitch::setup_common_emitter();
    raspComManager.setup();
    PING::player1.setup();
    PING::player2.setup();
    PING::player3.setup();
    PING::player4.setup();

    xTaskCreatePinnedToCore(
        PING::run_task,         /* Function to implement the task */
        "run_task",             /* Name of the task */
        10000,                                /* Stack size in words */
        NULL,                                 /* Task input parameter */
        TASK_BEAM_RECEIVER_PRIORITY,          /* Priority of the task */
        &PING::run_task_handle, /* Task handle. */
        TASK_BEAM_RECEIVER_CORE               /* Core where the task should run */
    );
}