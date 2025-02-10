#ifndef LINEAR_ACTUATOR_HPP
#define LINEAR_ACTUATOR_HPP
// #include <AccelStepper.h>
#include "FastAccelStepper.h"
#include <TMCStepper.h>
#include "config.h"
#include "vector.hpp"
#include <Arduino.h>

#define MICRO_STEPS_PER_MM ((float)STEPS_PER_REVOLUTION * (1 << MICROSTEP_POWER_OF_2) / (PULLEY_TEETH * BELT_PITCH))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define saturate(a, low, high) (min(max(a, low), high))

#define LINEAR_ACTUATOR_MIN_AMPLITUDE (LINEAR_ACTUATOR_RAILHEAD - MAX_BUMPER_WIDTH)

class LinearActuator
{
public:
    static void setup_all();
    static void stall_guard_task(void *pvParameters);
    static void motor_run_task(void *pvParameters);
    static FastAccelStepperEngine engine;

    LinearActuator(uint8_t stepPin, uint8_t dirPin, uint8_t addresss, bool shaftt = false) : driver(&TMC_SERIAL_PORT, TMC_R_SENSE, addresss), shaft(shaftt), stepPin(stepPin), dirPin(dirPin) {}
    ~LinearActuator() {};
    void setup();
    bool get_stall_result();
    void reset_right_limit() { rightLimit = -LINEAR_ACTUATOR_RAILHEAD; }
    void reset_left_limit() { leftLimit = LINEAR_ACTUATOR_RAILHEAD; }
    // void invert(bool shaft) { motor->setPinsInverted(shaft); }
    void set_max_speed(float speed) { motor->setSpeedInHz(min(speed, LINEAR_ACTUATOR_MAX_SPEED) * MICRO_STEPS_PER_MM); }
    // void set_max_speed(float speed) { motor->setAbsoluteSpeedLimit(min(speed, LINEAR_ACTUATOR_MAX_SPEED) * MICRO_STEPS_PER_MM); }
    void set_acceleration(float acceleration) { motor->setAcceleration(min(acceleration, LINEAR_ACTUATOR_MAX_ACCELERATION) * MICRO_STEPS_PER_MM); }
    bool move_to(float position);
    bool move(float relativePosition);
    void move_right() { move_to(rightLimit); }
    void move_left() { move_to(leftLimit); }
    void stop() { begin_mvt_flag = true; motor->stopMove(); }
    void instant_stop();
    void calibrate()
    {
        reset_right_limit();
        reset_left_limit();
        calibrating = true;
    }
    float current_position() { return (float)motor->getCurrentPosition() / MICRO_STEPS_PER_MM; }
    float current_speed() { return 1e6 / motor->getCurrentSpeedInUs() / MICRO_STEPS_PER_MM; }
    float current_acceleration() { return (float)motor->getCurrentAcceleration() / MICRO_STEPS_PER_MM; }
    float max_speed() { return 1e6 / motor->getSpeedInUs() / MICRO_STEPS_PER_MM; }
    float max_acceleration() { return (float)motor->getAcceleration() / MICRO_STEPS_PER_MM; }
    float amplitude() { return rightLimit - leftLimit; }
    float get_right_limit() { return rightLimit; }
    float get_left_limit() { return leftLimit; }
    bool is_right_calibrated() { return rightLimit > -LINEAR_ACTUATOR_RAILHEAD; }
    bool is_left_calibrated() { return leftLimit < LINEAR_ACTUATOR_RAILHEAD; }
    bool is_calibrated() { return is_right_calibrated() && is_left_calibrated(); }
    bool is_calibrating() { return calibrating; }
    bool is_busy() { return is_calibrating(); }
    bool consume_mvt_flag() {
        if (begin_mvt_flag && !motor->isRunning())
        {
            begin_mvt_flag = false;
            mvt_flag = true;
        }
        bool tmp = mvt_flag; mvt_flag = false; return tmp; }
    bool consume_cal_flag() { bool tmp = cal_flag; cal_flag = false; return tmp; }
    bool is_new_acceleration();
    bool is_new_ramp_state() {
        bool tmp = motor->rampState() != previousRampState;
        previousRampState = motor->rampState();
        return tmp;
    }


#ifndef EVERYTHING_PUBLIC
private:
#endif
    uint8_t stepPin, dirPin;
    float previousSpeed = 0;
    float previousTime = 0;
    float currentAcceleration = 0, previousAcceleration = 0;
    uint8_t previousRampState = RAMP_STATE_IDLE;

    static Vector<LinearActuator *> all;
    float rightLimit = -LINEAR_ACTUATOR_RAILHEAD;
    float leftLimit = LINEAR_ACTUATOR_RAILHEAD;
    TMC2209Stepper driver;
    FastAccelStepper *motor = NULL;
    void set_current_position(float position) { motor->setCurrentPosition(position * MICRO_STEPS_PER_MM); }
    int64_t chrono = 0;
    bool shaft;
    bool running = false;
    int currentCalibrationSteps = 0;
    float calibrationFirstWallPosition = 0, calibrationNewWallPosition = 0;
    int calibrationGoodSamples = 0;
    bool askForStallGuard = false;
    bool newStallGuardAvailable = false;
    uint8_t updateSgTh = 0;
    bool stallResult = false;
    bool calibrating = false;
    bool mvt_flag = false;
    bool begin_mvt_flag = false;
    bool cal_flag = false;
    // calibrationSteps
    bool c_step1();
    bool c_step2(int64_t time = esp_timer_get_time());
    bool c_step3(int64_t time = esp_timer_get_time());
    bool c_step4();
    bool c_step5();
    bool c_step6(int64_t time = esp_timer_get_time());
    bool c_step7(int64_t time = esp_timer_get_time());
    bool c_step8();
    bool c_step9();
    bool c_step10(int64_t time = esp_timer_get_time());
    bool c_step11(int64_t time = esp_timer_get_time());
    bool c_step12();
    bool c_step13();
    bool c_step14(int64_t time = esp_timer_get_time());
    bool c_step15(int64_t time = esp_timer_get_time());
    bool c_step16();

    bool calibration(int64_t time = esp_timer_get_time());
};

#endif
