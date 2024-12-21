#ifndef BEAM_SWITCH_HPP
#define BEAM_SWITCH_HPP

#include "config.h"
#include <Arduino.h>
#include <vector.hpp>

class BeamSwitch
{
public:
    BeamSwitch() = delete;
    BeamSwitch(int beamSwitchRPin);
    ~BeamSwitch();
    static void setup();
    static void startEmit();
    static void stopEmit();
    bool getState() { return state; };
    

#ifndef EVERYTHING_PUBLIC
private:
#endif    
    static bool emit;
    static Vector<BeamSwitch*> all;
    static TaskHandle_t check_all_task_handle;
    static void check_all_task(void *pvParameters);
    static void check_all(uint64_t currentTime = esp_timer_get_time());

    

    const int beamSwitchRPin;
    int lastReceiveTime = 0;
    bool state = false;
    bool check(uint64_t currentTime = esp_timer_get_time());

};

#endif