#include "Solenoid.hpp"
#include "config.h"
float Solenoid::maxTemp = 50.0;

Solenoid::Solenoid(int solenoidPin)
{
    this->solenoidPin = solenoidPin;
}

Solenoid::~Solenoid()
{
}

void Solenoid::setup()
{
    // pinMode(this->solenoidPin, OUTPUT);
    ledcAttachPin(this->solenoidPin, 0);
    ledcSetup(0, 100000, ANALOG_WRITE_RESOLUTION);
    deactivate();
}

void Solenoid::activate()
{
    state = true;
    // digitalWrite(this->solenoidPin, HIGH);
    int pwm = power * (1 << ANALOG_WRITE_RESOLUTION)-1;
    Serial.println(pwm);
    // analogWrite(this->solenoidPin, power);
    ledcWrite(0, pwm);
}

void Solenoid::deactivate()
{
    state = false;
    // digitalWrite(this->solenoidPin, LOW);
    // analogWrite(this->solenoidPin, 0);
    ledcWrite(0, 0);
}

bool Solenoid::overTempProtect(uint64_t currentTime)
{
    float dt = (currentTime - lastTempCheck) / 1000000.0;
    lastTempCheck = currentTime;

    if (state>0)
    {
        currentTemp += state * dt * 5;  // 0.1°C/s
        if (currentTemp > maxTemp)
        {
            deactivate();
            return true;
        }
    }
    else
    {
        currentTemp -= dt * 5;  // 0.05°C/s
        if (currentTemp < 0.0)
            currentTemp = 0.0;
    }
    return false;
}



