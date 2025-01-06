#include <Arduino.h>

// input
#define VOLUME_PIN A3
#define LEVEL_PIN A2
#define LIGHT_PIN A1

#define MODE_PB_PIN 7
#define MODE_PIN_A 6
#define MODE_PIN_B 5
#define RESET_PIN 4


#define VOLUME_KEY "volume"
#define LEVEL_KEY "level"
#define LIGHT_KEY "light"
#define MODE_PB_KEY "mode_pb"
#define MODE_KEY "mode"
#define RESET "reset"

#define PUSH_ACTION_KEY "push"
#define RELEASE_ACTION_KEY "release"
#define VALUE_ACTION_KEY "value"
#define INCREMENT_ACTION_KEY "increment"
#define DECREMENT_ACTION_KEY "decrement"

#define ANTI_NOISE_THRESHOLD 5
#define DEBOUNCE_DELAY 50
#define NB_MODES 8

// output
#define STATUS_LED 3

#define STATUS_LED_KEY "status_led"

#define STATUS_LED_ON "on"
#define STATUS_LED_OFF "off"