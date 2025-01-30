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
    bool reset;

    void refresh(Inputs inputs)
    {
        mode_pb = inputs.mode_pb;
        mode_a = inputs.mode_a;
        mode_b = inputs.mode_b;
        reset = inputs.reset;
    }
};

struct Outputs
{
    bool status_led;
};

struct Messages
{
    String key;
    String action;
    bool valid;
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
        digitalWrite(STATUS_LED, LOW);
    }
    void loop()
    {
        readInputs();
        sendInputs();
        // process inputs
        updateOutputs();
        // write outputs
        writeOutputs();
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
                send(MODE_KEY, DECREMENT_ACTION_KEY);
            }
            else // mode_b is 1: counter-clockwise
            {
                send(MODE_KEY, INCREMENT_ACTION_KEY);
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
            send(VOLUME_KEY, inputs.volume);
            lastInputs.volume = inputs.volume;
        }

        if (abs(inputs.level - lastInputs.level) > ANTI_NOISE_THRESHOLD)
        {
            send(LEVEL_KEY, inputs.level);
            lastInputs.level = inputs.level;
        }
        
        if (abs(inputs.light - lastInputs.light) > ANTI_NOISE_THRESHOLD)
        {
            send(LIGHT_KEY, inputs.light);
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
    void send(String key, T action) // method to send a message
    {
        Serial.print(key);
        Serial.print("/");
        Serial.println(action);
    }

    Messages receive() {
        Messages msg = {"", "", false}; // Initialise un message vide

        if (Serial.available() > 0) {
            String message = Serial.readStringUntil('\n'); // Lire la ligne jusqu'à '\n'
            // remove the '\n' character
            int separatorIndex = message.indexOf('/');

            if (separatorIndex != -1) {
                msg.key = message.substring(0, separatorIndex);
                msg.action = message.substring(separatorIndex + 1);
                msg.valid = true; // Le message est valide
            } else {
                msg.key = "Error";
                msg.action = "Invalid format";
                msg.valid = false;
                Serial.println("Invalid format");
            }
        }
        return msg;
    }

    void updateOutputs()
    {
        Messages msg = receive();
        if (!msg.valid) return;

        if (msg.key == STATUS_LED_KEY)
        {
            if (msg.action == STATUS_LED_ON)
                outputs.status_led = true;
            else if (msg.action == STATUS_LED_OFF)
                outputs.status_led = false;
        }
        // si on recoit un message ASK_STATUS_SETTINGS, on renvoie les valeurs des inputs
        if (msg.key == ASK_STATUS_SETTINGS)
        {
            lastInputs.volume = -ANTI_NOISE_THRESHOLD-1;
            lastInputs.level = -ANTI_NOISE_THRESHOLD-1;
            lastInputs.light = -ANTI_NOISE_THRESHOLD-1;             
        }
    }

    void writeOutputs()
    {
        if (outputs.status_led)
        {
            digitalWrite(STATUS_LED, HIGH);
        }
        else
        {
            digitalWrite(STATUS_LED, LOW);
        }
    }
    Inputs inputs, lastInputs;
    Outputs outputs;
};