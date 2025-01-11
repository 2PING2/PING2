#include "LinearActuator.hpp"

Vector<LinearActuator *> LinearActuator::all;

void LinearActuator::setup_Serial()
{
    TMC_SERIAL_PORT.begin(TMC_SERIAL_BAUD_RATE);
    xTaskCreatePinnedToCore(
        stall_guard_task,
        "stall_guard_task",
        10000,
        NULL,
        TASK_STALLGUARD_PRIORITY,
        NULL,
        TASK_STALLGUARD_CORE);
}

void LinearActuator::stall_guard_task(void *pvParameters)
{
    for (;;)
    {
        for (LinearActuator *la : LinearActuator::all)
        {
            if (la->askForStallGuard)
            {
                la->stallGuardValue = la->driver.SG_RESULT();
                la->newStallGuardAvailable = true;
            }
        }
        vTaskDelay(TASK_STALLGUARD_DELAY_MS / portTICK_PERIOD_MS);
    }
}

void LinearActuator::setup()
{
    all.push_back(this);
    set_max_speed(LINEAR_ACTUATOR_MAX_SPEED);           // set max speed
    set_acceleration(LINEAR_ACTUATOR_MAX_ACCELERATION); // set acceleration
    motor.enableOutputs();                              // enable motor outputs
    driver.begin();
    driver.senddelay(8); // not sure about this
    driver.toff(4);
    driver.blank_time(24);
    driver.rms_current(TMC_MAX_CURRENT);
    driver.mres(8 - MICROSTEP_POWER_OF_2);
    driver.TCOOLTHRS(0xFFFFF); // 20bit max
    driver.semin(5);
    driver.semax(2);
    driver.shaft(shaft);
    driver.pwm_autoscale(true); // Needed for stealthChop
}

uint16_t LinearActuator::get_stall_guard_value()
{
    askForStallGuard = true;
    if (!newStallGuardAvailable)
        return COARSE_CALIBRATION_STALL_VALUE + 1;
    else
    {
        newStallGuardAvailable = false;
        return stallGuardValue;
    }
}

void LinearActuator::instant_stop()
{
    motor.setSpeed(0);
    motor.runSpeed();
}

int LinearActuator::run()
{
    return motor.run();
}

bool LinearActuator::move_to(float position)
{
    motor.moveTo(position * MICRO_STEPS_PER_MM);
    return motor.distanceToGo() == 0;
}

bool LinearActuator::move(float relativePosition)
{
    motor.move(relativePosition * MICRO_STEPS_PER_MM);
    return motor.distanceToGo() == 0;
}

bool LinearActuator::c_step1()
{
    if (!true)
        return false;
    set_max_speed(COARSE_CALIBRATION_SPEED);
    move_left();
    return true;
}

bool LinearActuator::c_step2(int64_t time)
{
    if (!(COARSE_CALIBRATION_SPEED - abs(current_speed()) < 1e-1))
        return false;
    chrono = time;
    return true;
}

bool LinearActuator::c_step3(int64_t time)
{
    if (!(time - chrono > 20000 && get_stall_guard_value() < COARSE_CALIBRATION_STALL_VALUE))
        return false;
    instant_stop();
    askForStallGuard = false;
    calibrationFirstWallPosition = current_position();
    calibrationGoodSamples = 1;

    return true;
}

bool LinearActuator::c_step4()
{
    if (!true)
        return false;
    set_max_speed(LINEAR_ACTUATOR_MAX_SPEED);
    move(-FINE_CALIBRATION_WITHDRAWAL_DISTANCE);
    return true;
}

bool LinearActuator::c_step5()
{
    if (!motor.distanceToGo() == 0)
        return false;
    set_max_speed(FINE_CALIBRATION_SPEED);
    move_left();
    return true;
}

bool LinearActuator::c_step6(int64_t time)
{
    if (!(FINE_CALIBRATION_SPEED - abs(current_speed()) < 1e-1))
        return false;
    chrono = time;
    return true;
}

bool LinearActuator::c_step7(int64_t time)
{
    if (!(time - chrono > 20000 && get_stall_guard_value() < FINE_CALIBRATION_STALL_VALUE))
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

bool LinearActuator::c_step8()
{
    if (!true)
        return false; // don't forget to implement counter condition in main calibration method
    set_max_speed(LINEAR_ACTUATOR_MAX_SPEED);
    move(FINE_CALIBRATION_WITHDRAWAL_DISTANCE);
    return true;
}

bool LinearActuator::c_step9()
{
    if (!motor.distanceToGo() == 0)
        return false;
    set_max_speed(FINE_CALIBRATION_SPEED);
    move_right();
    return true;
}

bool LinearActuator::c_step10(int64_t time)
{
    if (!(FINE_CALIBRATION_SPEED - abs(current_speed()) < 1e-1))
        return false;
    chrono = time;
    return true;
}

bool LinearActuator::c_step11(int64_t time)
{
    if (!(time - chrono > 20000 && get_stall_guard_value() < FINE_CALIBRATION_STALL_VALUE))
        return false;
    askForStallGuard = false;
    instant_stop();
    calibrationFirstWallPosition = current_position();
    calibrationGoodSamples = 1;
    return true;
}

bool LinearActuator::c_step12()
{
    if (!true)
        return false; // don't forget to implement counter condition in main calibration method
    set_max_speed(LINEAR_ACTUATOR_MAX_SPEED);
    move(-LINEAR_ACTUATOR_MIN_AMPLITUDE);
    return true;
}

bool LinearActuator::c_step13()
{
    if (!motor.distanceToGo() == 0)
        return false;
    set_max_speed(FINE_CALIBRATION_SPEED);
    move_right();
    return true;
}

bool LinearActuator::c_step14(int64_t time)
{
    if (!(FINE_CALIBRATION_SPEED - abs(current_speed()) < 1e-1))
        return false;
    chrono = time;
    return true;
}

bool LinearActuator::c_step15(int64_t time)
{
    if (!(time - chrono > 20000 && get_stall_guard_value() < FINE_CALIBRATION_STALL_VALUE))
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

bool LinearActuator::c_step16()
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

bool LinearActuator::calibration(int64_t time)
{
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

            currentCalibrationSteps = 0;
            return true;
        }
        else
            currentCalibrationSteps = 11;
        break;
    default:
        break;
    }
    return false;
}
