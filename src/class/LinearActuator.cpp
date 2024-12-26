#include "LinearActuator.hpp"

void LinearActuator::setup()
{
    // driver.setMicrostepsPerStepPowerOfTwo(MICROSTEP_POWER_OF_2);
    // driver.setRunCurrent(RMS_CURRENT);
    // driver.setStallGuardThreshold(STALL_VALUE);
    set_max_speed(LINEAR_ACTUATOR_MAX_SPEED);            // set max speed
    set_acceleration(LINEAR_ACTUATOR_MAX_ACCELERATION); // set acceleration
    // motor.enableOutputs();                             // enable motor outputs
    // driver.enable();
    // driver.enableAutomaticCurrentScaling();
    driver.setRunCurrent(RMS_CURRENT);
    // driver.setStallGuardThreshold(STALL_VALUE);
    driver.setMicrostepsPerStepPowerOfTwo(MICROSTEP_POWER_OF_2);
    driver.enable();
}

bool LinearActuator::calibrate_right()
{
    set_max_speed(COARSE_CALIBRATION_SPEED);
    move_right();
    if (driver.getStallGuardResult() > COARSE_CALIBRATION_STALL_VALUE)
        return false;
    rightLimit = current_position();
    if (leftLimit > INT_MIN)
        motor.setCurrentPosition(amplitude() * MICRO_STEPS_PER_MM / 2);
    set_max_speed(LINEAR_ACTUATOR_MAX_SPEED);
    return true;
}

bool LinearActuator::check_right_calibration()
{
    
}

bool LinearActuator::calibrate_left()
{
    set_max_speed(COARSE_CALIBRATION_SPEED);
    move_left();
    if (driver.getStallGuardResult() > COARSE_CALIBRATION_STALL_VALUE)
        return false;
    leftLimit = current_position();
    if (rightLimit < INT_MAX)
        motor.setCurrentPosition( - amplitude() * MICRO_STEPS_PER_MM / 2);
    set_max_speed(LINEAR_ACTUATOR_MAX_SPEED);
    return true;
}

void LinearActuator::instant_stop()
{
    motor.setSpeed(0);
    motor.runSpeed();
}

int LinearActuator::run()
{
    motor.run();
    return RunStatus::RUNNING;
}

