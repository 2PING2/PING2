#ifndef SOLENOID_HPP
#define SOLENOID_HPP

#include <Arduino.h>

class Solenoid
{
private:
    /* data */
    float state = 0, power = 0.82;
    int solenoidPin;
    static float maxTemp;
    float currentTemp = 0.0;
    uint64_t lastTempCheck = 0;
public:
    Solenoid(int solenoidPin),
    ~Solenoid();
    void setup();
    void activate();
    void deactivate();
    void set_power(int power){this->power = power;}
    float get_power(){return power;}

    float get_state(){return state;}

    bool over_temp_protect(uint64_t currentTime = esp_timer_get_time());
};

#endif