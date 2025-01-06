#include "config.hpp" 
#include <Arduino.h>
#pragma once
unsigned long lastDebounceTimeA = 0; // Anti-bounce timer for mode_a
unsigned long lastDebounceTimeB = 0; // Anti-bounce timer for mode_b
bool stableModeA = false;
bool stableModeB = false;
unsigned long lastDebounceTimePB = 0; // Anti-bounce timer for mode_pb
bool stableModePB = false;            // État stable de mode_pb
bool lastModeA = false;
bool lastModeB = false;


struct Inputs
{
    int volume;
    int level;
    int light;
    bool mode_pb;
    bool mode_a, mode_b;
    int mode;
    bool reset;

    void refresh(Inputs inputs)
    {
        mode_pb = inputs.mode_pb;
        mode_a = inputs.mode_a;
        mode_b = inputs.mode_b;
        mode = inputs.mode;
        reset = inputs.reset;
    }
};

struct Outputs
{
    bool status_led;
};

class Corner
{
public:
    Corner(){};

    void setup()
    {
        pinMode(VOLUME_PIN, INPUT);
        pinMode(LEVEL_PIN, INPUT);
        pinMode(LIGHT_PIN, INPUT);
        pinMode(MODE_PB_PIN, INPUT_PULLUP);
        pinMode(MODE_PIN_A, INPUT_PULLUP);
        pinMode(MODE_PIN_B, INPUT_PULLUP);
        pinMode(RESET_PIN, INPUT_PULLUP);
        pinMode(STATUS_LED, OUTPUT);
    }
    void loop()
    {
        readInputs();
        sendInputs();
        // process inputs
        // update outputs
        // write outputs
        // writeOutputs();
    }

    void handleEncoder()
    {
        // Read the current state of mode_a and mode_b
        bool currentModeA = inputs.mode_a;
        bool currentModeB = inputs.mode_b;

        // Detection of a rising edge on mode_a
        if (lastModeA == false && currentModeA == true) 
        {
            if (currentModeB == false) // mode_b is 0: clockwise
            {
                inputs.mode--;
                if (inputs.mode < 0)
                {
                    inputs.mode = NB_MODES; // Cycle to NB_MODES if below 0
                }
                send(MODE_KEY, DECREMENT_ACTION_KEY, inputs.mode);
            }
            else // mode_b is 1: counter-clockwise
            {
                inputs.mode++;
                if (inputs.mode > NB_MODES)
                {
                    inputs.mode = 0; // Cycle to 0 if NB_MODES is exceeded
                }
                send(MODE_KEY, INCREMENT_ACTION_KEY, inputs.mode);
            }
        }

        // Update the last state of mode_a and mode_b
        lastModeA = currentModeA;
        lastModeB = currentModeB;
    }

private:
    void readInputs()
    {
        inputs.volume = analogRead(VOLUME_PIN);
        inputs.level = analogRead(LEVEL_PIN);
        inputs.light = analogRead(LIGHT_PIN);
        inputs.mode_pb = !digitalRead(MODE_PB_PIN);
        inputs.mode_a = !digitalRead(MODE_PIN_A);
        inputs.mode_b = !digitalRead(MODE_PIN_B);
        inputs.reset = !digitalRead(RESET_PIN);
    }

    void sendInputs()
    {
        if (abs(inputs.volume - lastInputs.volume) > ANTI_NOISE_THRESHOLD)
        {
            send(VOLUME_KEY, VALUE_ACTION_KEY, inputs.volume);
            lastInputs.volume = inputs.volume;
        }

        if (abs(inputs.level - lastInputs.level) > ANTI_NOISE_THRESHOLD)
        {
            send(LEVEL_KEY, VALUE_ACTION_KEY, inputs.level);
            lastInputs.level = inputs.level;
        }
        
        if (abs(inputs.light - lastInputs.light) > ANTI_NOISE_THRESHOLD)
        {
            send(LIGHT_KEY, VALUE_ACTION_KEY, inputs.light);
            lastInputs.light = inputs.light;
        }

        

        // Mode PB
        if (inputs.mode_pb != stableModePB && millis() - lastDebounceTimePB > DEBOUNCE_DELAY)
        {
            lastDebounceTimePB = millis(); // Reset the anti-bounce timer
            stableModePB = inputs.mode_pb; // Update the stable state of mode_pb

            if (stableModePB)
            {
                send(MODE_PB_KEY, PUSH_ACTION_KEY);
            }
            else
            {
                send(MODE_PB_KEY, RELEASE_ACTION_KEY);
            }
        }

        
        if (stableModePB){
            handleEncoder(); // Handle the encoder only if mode_pb is pressed
        }
        
                
        

        

        // Reset

        if (inputs.reset != lastInputs.reset) 
        {
            if (inputs.reset)
            {
                send(RESET, PUSH_ACTION_KEY);
            }
            else
            {
                send(RESET, RELEASE_ACTION_KEY);
            }
        }
        lastInputs.refresh(inputs);
    }

    template <typename T> 
    void send(String key, String action, T value) // method to send a message
    {
        Serial.print(key);
        Serial.print("/");
        Serial.print(action);
        Serial.print("/");
        Serial.println(value);
    }
    void send(String key, String action)
    {
        Serial.print(key);
        Serial.print("/");
        Serial.println(action);
    }
    Inputs inputs, lastInputs;
    Outputs outputs;
};