#include "BeamSwitch.hpp"

bool BeamSwitch::emit = false;

BeamSwitch::BeamSwitch(int beamSwitchRPin) : beamSwitchRPin(beamSwitchRPin)
{
}


void BeamSwitch::setup_common_emitter()
{
    pinMode(BEAM_T_PIN, OUTPUT);
    ledcSetup(IR_PWM_CHANNEL, IR_PWM_FREQUENCY, 8);
    ledcAttachPin(BEAM_T_PIN, IR_PWM_CHANNEL);
    ledcWrite(IR_PWM_CHANNEL, 0);
}

void BeamSwitch::startEmit()
{
    emit = true;
    ledcWrite(IR_PWM_CHANNEL, 128);
}

void BeamSwitch::stopEmit()
{
    emit = false;
    ledcWrite(IR_PWM_CHANNEL, 0);
}


void BeamSwitch::setup()
{
    pinMode(beamSwitchRPin, INPUT);
    if (emit)
        startEmit();
    else
        stopEmit();
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
