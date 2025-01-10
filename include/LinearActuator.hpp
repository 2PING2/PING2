#ifndef LINEAR_ACTUATOR_HPP
#define LINEAR_ACTUATOR_HPP
#include <AccelStepper.h>
#include <TMCStepper.h>
#include "config.h"
#include "vector.hpp"
#include <Arduino.h>

#define MICRO_STEPS_PER_MM ((float)STEPS_PER_REVOLUTION * (1 << MICROSTEP_POWER_OF_2) / (PULLEY_TEETH * BELT_PITCH))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define saturate(a, low, high) (min(max(a, low), high))

#define LINEAR_ACTUATOR_MIN_AMPLITUDE (LINEAR_ACTUATOR_RAILHEAD - MAX_BUMPER_WIDTH)
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
    static void setup_Serial() { 
        TMC_SERIAL_PORT.begin(TMC_SERIAL_BAUD_RATE); 
        xTaskCreatePinnedToCore(stallGuardTask, "stallGuardTask", 10000, NULL, TASK_STALLGUARD_PRIORITY, NULL, TASK_STALLGUARD_CORE);

    }
    // stallGuardTask
    static void stallGuardTask(void *pvParameters)
    {
        for (;;)
        {   
            for (LinearActuator *la : LinearActuator::all)
            {
                if (la->askForStallGuard)
                {
                    // Serial.print("psg ");
                    // Serial.println(LinearActuator::all.findFirst(la));
                    la->stallGuardValue = la->driver.SG_RESULT();
                    la->newStallGuardAvailable = true;
                }
            }
            vTaskDelay(TASK_STALLGUARD_DELAY_MS / portTICK_PERIOD_MS);
        }
    }
    // static 
    LinearActuator(int stepPin, int dirPin, uint8_t addresss, bool shaftt = false) : motor(AccelStepper::DRIVER, stepPin, dirPin), driver(&TMC_SERIAL_PORT, TMC_R_SENSE, addresss), shaft(shaftt) {}
    ~LinearActuator() {};
    void setup();
    int calibrate_right();
    int calibrate_left();
    uint16_t get_stallGuardValue() { 
        askForStallGuard = true; 
        if (!newStallGuardAvailable)
            return COARSE_CALIBRATION_STALL_VALUE+1;
        else
        {
            newStallGuardAvailable = false;
            return stallGuardValue;
        }
        }
    void reset_right_limit() { rightLimit = INT_MIN; }
    void reset_left_limit() { leftLimit = INT_MAX; }
    // bool check_right_calibration();
    // bool check_left_calibration();
    void invert(bool shaft) { motor.setPinsInverted(shaft); }
    void set_speed(float speed) { motor.setSpeed(min(speed, LINEAR_ACTUATOR_MAX_SPEED) * MICRO_STEPS_PER_MM); }
    void set_max_speed(float speed) { motor.setMaxSpeed(min(speed, LINEAR_ACTUATOR_MAX_SPEED) * MICRO_STEPS_PER_MM); }
    void set_acceleration(float acceleration) { motor.setAcceleration(min(acceleration, LINEAR_ACTUATOR_MAX_ACCELERATION) * MICRO_STEPS_PER_MM); }
    bool move_to(float position)
    {
        motor.moveTo(position * MICRO_STEPS_PER_MM);
        return motor.distanceToGo() == 0;
    }
    bool move(float relativePosition)
    {
        motor.move(relativePosition * MICRO_STEPS_PER_MM);
        return motor.distanceToGo() == 0;
    }
    void move_right() { move_to(rightLimit); }
    void move_left() { move_to(leftLimit); }
    void stop() { motor.stop(); }
    void instant_stop();
    int run();
    float current_position() { return motor.currentPosition() / MICRO_STEPS_PER_MM; }
    float current_speed() { return motor.speed() / MICRO_STEPS_PER_MM; }
    float max_speed() { return motor.maxSpeed() / MICRO_STEPS_PER_MM; }
    float amplitude() { return rightLimit - leftLimit; }
    bool is_right_calibrated() { return rightLimit > -LINEAR_ACTUATOR_RAILHEAD; }
    bool is_left_calibrated() { return leftLimit < LINEAR_ACTUATOR_RAILHEAD; }
    bool is_calibrated() { return is_right_calibrated() && is_left_calibrated(); }

#ifndef EVERYTHING_PUBLIC
private:
#endif
    static Vector<LinearActuator*> all;
    float rightLimit = -LINEAR_ACTUATOR_RAILHEAD;
    float leftLimit = LINEAR_ACTUATOR_RAILHEAD;
    TMC2209Stepper driver;
    AccelStepper motor;
    void set_current_position(float position) { motor.setCurrentPosition(position * MICRO_STEPS_PER_MM); }
    RunStatus::Status status = RunStatus::IDLE;
    int64_t chrono = 0;
    bool shaft;
    int currentCalibrationSteps = 0;
    float calibrationFirstWallPosition = 0, calibrationNewWallPosition = 0;
    int calibrationGoodSamples = 0;
    bool askForStallGuard = false;
    bool newStallGuardAvailable = false;
    int16_t stallGuardValue = COARSE_CALIBRATION_STALL_VALUE+1;

    // calibrationSteps
    bool c_step1()
    {
        if (!true)
            return false;
        set_max_speed(COARSE_CALIBRATION_SPEED);
        move_left();
        return true;
    }

    bool c_step2(int64_t time = esp_timer_get_time())
    {

        if (!(COARSE_CALIBRATION_SPEED - abs(current_speed()) < 1e-1))
            return false;
        chrono = time;
        return true;
    }

    bool c_step3(int64_t time = esp_timer_get_time())
    {
        if (!(time - chrono > 20000 && get_stallGuardValue() < COARSE_CALIBRATION_STALL_VALUE))
            return false;
        instant_stop();
        askForStallGuard = false;
        calibrationFirstWallPosition = current_position();
        calibrationGoodSamples = 1;

        return true;
    }

    bool c_step4()
    {
        if (!true)
            return false;
        set_max_speed(LINEAR_ACTUATOR_MAX_SPEED);
        move(-FINE_CALIBRATION_WITHDRAWAL_DISTANCE);
        return true;
    }

    bool c_step5()
    {
        if (!motor.distanceToGo() == 0)
            return false;
        set_max_speed(FINE_CALIBRATION_SPEED);
        move_left();
        return true;
    }

    bool c_step6(int64_t time = esp_timer_get_time())
    {
        if (!(FINE_CALIBRATION_SPEED - abs(current_speed()) < 1e-1))
            return false;
        chrono = time;
        return true;
    }

    bool c_step7(int64_t time = esp_timer_get_time())
    {
        if (!(time - chrono > 20000 && get_stallGuardValue() < FINE_CALIBRATION_STALL_VALUE))
            return false;
        askForStallGuard = false;
        instant_stop();
        calibrationNewWallPosition = current_position();
        if (abs(calibrationNewWallPosition - calibrationFirstWallPosition) < FINE_CALIBRATION_ERROR_THRESHOLD)
            calibrationGoodSamples++;
        else
        {
            calibrationGoodSamples = 1;
            calibrationFirstWallPosition = calibrationNewWallPosition;
        }
        return true;
    }

    bool c_step8()
    {
        if (calibrationGoodSamples < FINE_CALIBRATION_SAMPLES)
            return false; // don't forget to implement counter condition in main calibration method
        leftLimit = calibrationFirstWallPosition;
        set_max_speed(LINEAR_ACTUATOR_MAX_SPEED);
        move(-LINEAR_ACTUATOR_MIN_AMPLITUDE);
        return true;
    }

    bool c_step9()
    {
        if (!motor.distanceToGo() == 0)
            return false;
        set_max_speed(COARSE_CALIBRATION_SPEED);
        move_right();
        return true;
    }

    bool c_step10(int64_t time = esp_timer_get_time())
    {
        if (!(COARSE_CALIBRATION_SPEED - abs(current_speed()) < 1e-1))
            return false;
        chrono = time;
        return true;
    }

    bool c_step11(int64_t time = esp_timer_get_time())
    {
        if (!(time - chrono > 20000 && get_stallGuardValue() < COARSE_CALIBRATION_STALL_VALUE))
            return false;
        askForStallGuard = false;
        instant_stop();
        calibrationFirstWallPosition = current_position();
        calibrationGoodSamples = 1;
        return true;
    }

    bool c_step12()
    {
        if (!true)
            return false; // don't forget to implement counter condition in main calibration method
        set_max_speed(LINEAR_ACTUATOR_MAX_SPEED);
        move(FINE_CALIBRATION_WITHDRAWAL_DISTANCE);
        return true;
    }

    bool c_step13()
    {
        if (!motor.distanceToGo() == 0)
            return false;
        set_max_speed(FINE_CALIBRATION_SPEED);
        move_right();
        return true;
    }

    bool c_step14(int64_t time = esp_timer_get_time())
    {
        if (!(FINE_CALIBRATION_SPEED - abs(current_speed()) < 1e-1))
            return false;
        chrono = time;
        return true;
    }

    bool c_step15(int64_t time = esp_timer_get_time())
    {
        if (!(time - chrono > 20000 && get_stallGuardValue() < FINE_CALIBRATION_STALL_VALUE))
            return false;
        askForStallGuard = false;
        instant_stop();
        calibrationNewWallPosition = current_position();
        if (abs(calibrationNewWallPosition - calibrationFirstWallPosition) < FINE_CALIBRATION_ERROR_THRESHOLD)
            calibrationGoodSamples++;
        else
        {
            calibrationGoodSamples = 1;
            calibrationFirstWallPosition = calibrationNewWallPosition;
        }
        return true;
    }

    bool c_step16()
    {
        if (calibrationGoodSamples < FINE_CALIBRATION_SAMPLES)
            return false;
        rightLimit = calibrationFirstWallPosition;
        float amplitude = leftLimit - rightLimit;

        rightLimit = -amplitude / 2 + NO_COLLISION_MARGIN;
        leftLimit = amplitude / 2 - NO_COLLISION_MARGIN;
        
        motor.setCurrentPosition(-amplitude * MICRO_STEPS_PER_MM / 2);
        set_max_speed(LINEAR_ACTUATOR_MAX_SPEED);
        move_to(0);
        return true;
    }

    bool calibration(int64_t time = esp_timer_get_time())
    {
        // Serial.print("c");
        switch (currentCalibrationSteps)
        {
        case 0:
            if (c_step1())
                currentCalibrationSteps++;
            break;
        case 1:
            if (c_step2(time))
                currentCalibrationSteps++;
            break;
        case 2:
            if (c_step3(time))
                currentCalibrationSteps++;
            break;
        case 3:
            if (c_step4())
                currentCalibrationSteps++;
            break;
        case 4:
            if (c_step5())
                currentCalibrationSteps++;
            break;
        case 5:
            if (c_step6(time))
                currentCalibrationSteps++;
            break;
        case 6:
            if (c_step7(time))
                currentCalibrationSteps++;
            break;
        case 7:
            if (c_step8())
                currentCalibrationSteps++;
            else
                currentCalibrationSteps = 3;
            break;
        case 8:
            if (c_step9())
                currentCalibrationSteps++;
            break;
        case 9:
            if (c_step10(time))
                currentCalibrationSteps++;
            break;
        case 10:
            if (c_step11(time))
                currentCalibrationSteps++;
            break;
        case 11:
            if (c_step12())
                currentCalibrationSteps++;
            break;
        case 12:
            if (c_step13())
                currentCalibrationSteps++;
            break;
        case 13:
            if (c_step14(time))
                currentCalibrationSteps++;
            break;
        case 14:
            if (c_step15(time))
                currentCalibrationSteps++;
            break;
        case 15:
            if (c_step16())
            {
                
                currentCalibrationSteps= 0;
                return true;
            }
            else
                currentCalibrationSteps = 11;
            break;
        default:
        {
            // Serial.println("bad calibration step");
        }
        }
        return false;
    }
};

#endif
