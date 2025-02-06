#include "BeamSwitch.hpp"

bool BeamSwitch::emit = false;

Vector<BeamSwitch*> BeamSwitch::all;

TaskHandle_t BeamSwitch::checkAllTaskHandle;

BeamSwitch::BeamSwitch(int beamSwitchRPin) : beamSwitchRPin(beamSwitchRPin)
{
    // all.push_back(this);
}

BeamSwitch::~BeamSwitch()
{
    all.removeFirst(this);
}


void BeamSwitch::start_emit()
{
    emit = true;
    ledcWrite(IR_PWM_CHANNEL, 128);
}

void BeamSwitch::stop_emit()
{
    emit = false;
    ledcWrite(IR_PWM_CHANNEL, 0);
}

void BeamSwitch::check_all_task(void *pvParameters)
{
    for (;;)
    {
        uint64_t currentTime = esp_timer_get_time();
        BeamSwitch::check_all(currentTime);
        vTaskDelay(TASK_BEAM_CHECK_DELAY_MS / portTICK_PERIOD_MS);
    }
}

void BeamSwitch::setup()
{
    pinMode(beamSwitchRPin, INPUT);
    all.push_back(this);
}


void BeamSwitch::setup_emitter()
{
    pinMode(BEAM_T_PIN, OUTPUT);
    ledcSetup(IR_PWM_CHANNEL, IR_PWM_FREQUENCY, 8);
    ledcAttachPin(BEAM_T_PIN, IR_PWM_CHANNEL);
    // ledcWrite(IR_PWM_CHANNEL, 0);
    start_emit();

    // for (int i = 0; i < all.size(); i++)
    //     pinMode(all[i]->beamSwitchRPin, INPUT);

    xTaskCreatePinnedToCore(
        BeamSwitch::check_all_task,         /* Function to implement the task */
        "check_all_task",             /* Name of the task */
        10000,                                /* Stack size in words */
        NULL,                                 /* Task input parameter */
        TASK_BEAM_CHECK_PRIORITY,          /* Priority of the task */
        &checkAllTaskHandle, /* Task handle. */
        TASK_BEAM_CHECK_CORE               /* Core where the task should run */
    );
}

void BeamSwitch::check_all(uint64_t currentTime)
{
    // Serial.println(all.size());
    for (int i = 0; i < all.size(); i++)
        all[i]->check(currentTime);
}


bool BeamSwitch::check(uint64_t currentTime)
{
    if ((GPIO.in & (1 << beamSwitchRPin)) == 0)
        lastReceiveTime = currentTime;
    
    int dt = currentTime - lastReceiveTime;
    if (dt > BEAM_SWITCH_TIMEOUT_MS * 1000)
        state = true;
    else
        state = false;

    return state;
}
