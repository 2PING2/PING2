#ifndef BEAM_SWITCH_HPP
#define BEAM_SWITCH_HPP

#include "config.h"
#include <Arduino.h>

class BeamSwitch
{
public:
    BeamSwitch(int beamSwitchRPin);
    ~BeamSwitch() {};
    static void setup_common_emitter();
    static void startEmit();
    static void stopEmit();
    void setup();
    bool check(uint64_t currentTime = esp_timer_get_time());
    bool getState() { return state; };
    

#ifndef EVERYTHING_PUBLIC
private:
#endif    
    static bool emit;
    const int beamSwitchRPin;
    int lastReceiveTime = 0;
    bool state = false;
};

#endif