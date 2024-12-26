#ifndef LINEAR_ACTUATOR_HPP
#define LINEAR_ACTUATOR_HPP
#include <AccelStepper.h>
#include <TMC2209.h>
#include "config.h"

#define MICRO_STEPS_PER_MM (float)STEPS_PER_REVOLUTION / PULLEY_TEETH / BELT_PITCH * (1 << MICROSTEP_POWER_OF_2)
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define saturate(a, low, high) (min(max(a, low), high))

namespace RunStatus
{
    enum Status
    {
        IDLE,
        RUNNING,
        COLLISION
    };
}

class LinearActuator
{
public:
    static void setup_Serial() { TMC_SERIAL_PORT.begin(TMC_SERIAL_BAUD_RATE); }
    LinearActuator(int stepPin, int dirPin, TMC2209::SerialAddress address) : motor(AccelStepper::DRIVER, stepPin, dirPin) {driver.setup(TMC_SERIAL_PORT, TMC_SERIAL_BAUD_RATE, address);};
    ~LinearActuator() {};
    void setup();
    bool calibrate_right();
    bool calibrate_left();
    bool check_right_calibration();
    // bool check_left_calibration();
    void set_speed(float speed) { motor.setSpeed(min(speed, LINEAR_ACTUATOR_MAX_SPEED) * MICRO_STEPS_PER_MM); }
    void set_max_speed(float speed) { motor.setMaxSpeed(min(speed, LINEAR_ACTUATOR_MAX_SPEED) * MICRO_STEPS_PER_MM); }
    void set_acceleration(float acceleration) { motor.setAcceleration(min(acceleration, LINEAR_ACTUATOR_MAX_ACCELERATION) * MICRO_STEPS_PER_MM); }
    bool move_to(float position) { motor.moveTo(position*MICRO_STEPS_PER_MM); return motor.distanceToGo() == 0; }
    void move_right() { move_to(rightLimit); }
    void move_left() { move_to(leftLimit); }
    void stop() { motor.stop(); }
    void instant_stop();
    int run();
    float current_position() { return motor.currentPosition() / MICRO_STEPS_PER_MM; }
    float current_speed() { return motor.speed() / MICRO_STEPS_PER_MM; }
    float current_acceleration() { return motor.acceleration() / MICRO_STEPS_PER_MM; }
    float max_speed() { return motor.maxSpeed() / MICRO_STEPS_PER_MM; }
    float amplitude() { return rightLimit - leftLimit; }
    bool is_right_calibrated() { return rightLimit < INT_MAX; }
    bool is_left_calibrated() { return leftLimit > INT_MIN; }
    bool is_calibrated() { return is_right_calibrated() && is_left_calibrated(); }

#ifndef EVERYTHING_PUBLIC
private:
#endif
    float rightLimit = INT_MAX;
    float leftLimit = INT_MIN;
    TMC2209 driver;
    AccelStepper motor;
    void set_current_position(float position) { motor.setCurrentPosition(position * MICRO_STEPS_PER_MM); }
    RunStatus::Status status = RunStatus::IDLE;
};

#endif