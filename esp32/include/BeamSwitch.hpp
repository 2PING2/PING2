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
    static void setup_emitter();
    static void start_emit();
    static void stop_emit();
    void setup();
    bool get_state() { return state; };
    bool isNewState() { bool tmp = lastState; lastState = state; return state != tmp;};
    

#ifndef EVERYTHING_PUBLIC
private:
#endif    
    static bool emit;
    static Vector<BeamSwitch*> all;
    static TaskHandle_t checkAllTaskHandle;
    static void check_all_task(void *pvParameters);
    static void check_all(uint64_t currentTime = esp_timer_get_time());

    

    const int beamSwitchRPin;
    int lastReceiveTime = 0;
    bool state = false;
    bool lastState = false;
    bool check(uint64_t currentTime = esp_timer_get_time());

};

#endif