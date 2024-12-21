#include "BeamSwitch.hpp"

IRsend BeamSwitch::emitter = IRsend(BEAM_T_PIN);
bool BeamSwitch::emit = true;
int64_t BeamSwitch::lastEmitTime = 0;
TaskHandle_t BeamSwitch::emit_task_handle;
uint16_t BeamSwitch::rawSignal[] = {300, 700};  // Un signal de X µs on, Y µs off


void BeamSwitch::emit_task(void *pvParameters)
{
    for (;;)
    {
        if (emit)
        {
            // emitter.sendRaw(rawSignal, 2, 38);  // 38kHz
            emitter.sendNEC(0x20DF10EF, 38);
            lastEmitTime = esp_timer_get_time();
        }
        vTaskDelay(TASK_BEAM_EMITTER_DELAY_MS / portTICK_PERIOD_MS);  // Short delay for timing
    }
}

void BeamSwitch::setup_common_emitter()
{
    emitter.begin();           // Start the IR LED
    xTaskCreatePinnedToCore (
    BeamSwitch::emit_task,   /* Function to implement the task */
    "emit_task",                     /* Name of the task */
    1000,                           /* Stack size in words */
    NULL,                            /* Task input parameter */
    TASK_BEAM_EMITTER_PRIORITY,                               /* Priority of the task */
    &BeamSwitch::emit_task_handle,  /* Task handle. */
    TASK_BEAM_EMITTER_CORE           /* Core where the task should run */
);
}


void BeamSwitch::setup()
{
    irRecv.enableIRIn();
}

void BeamSwitch::check(uint64_t currentTime)
{
    if (irRecv.decode(&results))
        {
            irRecv.resume();
            lastReceiveTime = currentTime;
        }
    
    int dt = lastEmitTime - lastReceiveTime;
    if (dt > BEAM_SWITCH_TIMEOUT_MS * 1000)
        state = true;
    else
        state = false;
}
