#include "Ping.hpp"

TaskHandle_t PING::solenoid_overtemp_task_handle;

RaspComManagement PING::raspComManager = RaspComManagement(RASP_BAUD_RATE);

void PING::solenoid_overtemp_task(void *pvParameters)
{
    for (;;)
    {
        PING::player1.solenoid.overTempProtect();
        PING::player2.solenoid.overTempProtect();
        PING::player3.solenoid.overTempProtect();
        PING::player4.solenoid.overTempProtect();
        vTaskDelay(TASK_SOLENOID_OVERTEMP_DELAY_MS / portTICK_PERIOD_MS);
    }
}


void PING::setup()
{
    analogWriteResolution(ANALOG_WRITE_RESOLUTION);
    BeamSwitch::setup();
    raspComManager.setup();
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
        &PING::solenoid_overtemp_task_handle, /* Task handle. */
        TASK_SOLENOID_OVERTEMP_CORE           /* Core where the task should run */
    );
}