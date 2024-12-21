#ifndef BEAM_SWITCH_HPP
#define BEAM_SWITCH_HPP

#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRrecv.h>
#include <IRutils.h>
#include "config.h"

class BeamSwitch
{
public:
    static bool emit;
    BeamSwitch(int beamSwitchRPin) : irRecv(beamSwitchRPin) {};
    ~BeamSwitch() {};
    static void setup_common_emitter();
    void setup();
    void check(uint64_t currentTime = esp_timer_get_time());
    bool getState() { return state; };

#ifndef EVERYTHING_PUBLIC
private:
#endif
    static IRsend emitter; // Initialize the IR LED sender
    static TaskHandle_t emit_task_handle;
    static int64_t lastEmitTime;
    static uint16_t rawSignal[];
    static void emit_task(void *pvParameters);
    int lastReceiveTime = 0;
    bool state = false;
    IRrecv irRecv;          // Initialize the IR receiver
    decode_results results; // Struct for decoded IR data
    TaskHandle_t receive_task_handle;
};

#endif