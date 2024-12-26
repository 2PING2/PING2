#include "LinearActuator.hpp"

void LinearActuator::setup()
{
    // driver.setMicrostepsPerStepPowerOfTwo(MICROSTEP_POWER_OF_2);
    // driver.setRunCurrent(RMS_CURRENT);
    // driver.setStallGuardThreshold(STALL_VALUE);
    setMaxSpeed(LINEAR_ACTUATOR_MAX_SPEED);            // set max speed
    setAcceleration(LINEAR_ACTUATOR_MAX_ACCELERATION); // set acceleration
    // motor.enableOutputs();                             // enable motor outputs
    // driver.enable();
    // driver.enableAutomaticCurrentScaling();
    driver.setRunCurrent(RMS_CURRENT);
    // driver.setStallGuardThreshold(STALL_VALUE);
    driver.setMicrostepsPerStepPowerOfTwo(MICROSTEP_POWER_OF_2);
    driver.enable();
}

bool LinearActuator::calibrateRight()
{
    setMaxSpeed(COARSE_CALIBRATION_SPEED);
    moveRight();
    if (driver.getStallGuardResult() > COARSE_CALIBRATION_STALL_VALUE)
        return false;
    rightLimit = currentPosition();
    if (leftLimit > INT_MIN)
        motor.setCurrentPosition(amplitude() * MICRO_STEPS_PER_MM / 2);
    setMaxSpeed(LINEAR_ACTUATOR_MAX_SPEED);
    return true;
}

bool LinearActuator::checkRightCalibration()
{
    
}

bool LinearActuator::calibrateLeft()
{
    setMaxSpeed(COARSE_CALIBRATION_SPEED);
    moveLeft();
    if (driver.getStallGuardResult() > COARSE_CALIBRATION_STALL_VALUE)
        return false;
    leftLimit = currentPosition();
    if (rightLimit < INT_MAX)
        motor.setCurrentPosition( - amplitude() * MICRO_STEPS_PER_MM / 2);
    setMaxSpeed(LINEAR_ACTUATOR_MAX_SPEED);
    return true;
}

void LinearActuator::instantStop()
{
    motor.setSpeed(0);
    motor.runSpeed();
}

int LinearActuator::run()
{
    motor.run();
    return RunStatus::RUNNING;
}

