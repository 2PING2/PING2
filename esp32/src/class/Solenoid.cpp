#include "Solenoid.hpp"
#include "config.h"
float Solenoid::maxTemp = 50.0;
uint8_t Solenoid::channelCount = 1;

Solenoid::Solenoid(int solenoidPin)
{
    this->solenoidPin = solenoidPin;
    channel = channelCount++;
}

Solenoid::~Solenoid()
{
}

void Solenoid::setup()
{
    ledcAttachPin(this->solenoidPin, channel);
    ledcSetup(channel, 100000, ANALOG_WRITE_RESOLUTION);
    deactivate();
}

void Solenoid::activate()
{
    state = true;
    int pwm = (this->power * (1 - MIN_SOLENOID_MIN_POWER) + MIN_SOLENOID_MIN_POWER) * ((1 << ANALOG_WRITE_RESOLUTION)-1);
    ledcWrite(channel, pwm);
}

void Solenoid::deactivate()
{
    state = false;
    ledcWrite(channel, 0);
}

bool Solenoid::over_temp_protect(uint64_t currentTime)
{
    float dt = (currentTime - lastTempCheck) / 1000000.0;
    lastTempCheck = currentTime;

    if (state>0)
    {
        currentTemp += state * dt * 5;  // 5°C/s
        if (currentTemp > maxTemp)
        {
            deactivate();
            return true;
        }
    }
    else
    {
        currentTemp -= dt * 5;  // -5°C/s
        if (currentTemp < 0.0)
            currentTemp = 0.0;
    }
    return false;
}



