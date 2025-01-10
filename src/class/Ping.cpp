#include "Ping.hpp"

TaskHandle_t PING::solenoidOvertempTaskHandle;

RaspComManagement PING::raspComManager = RaspComManagement(RASP_BAUD_RATE);

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
    BeamSwitch::setup_emitter();
    raspComManager.setup();
    LinearActuator::setup_Serial();
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
}