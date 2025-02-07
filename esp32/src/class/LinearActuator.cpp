#include "LinearActuator.hpp"

Vector<LinearActuator *> LinearActuator::all;
FastAccelStepperEngine LinearActuator::engine = FastAccelStepperEngine();

void LinearActuator::setup_all()
{
    engine.init();
    TMC_SERIAL_PORT.begin(TMC_SERIAL_BAUD_RATE);
    xTaskCreatePinnedToCore(
        stall_guard_task,
        "stall_guard_task",
        10000,
        NULL,
        TASK_STALLGUARD_PRIORITY,
        NULL,
        TASK_STALLGUARD_CORE);


    xTaskCreatePinnedToCore(
        motor_run_task,
        "motor_run_task",
        10000,
        NULL,
        TASK_MOTOR_RUN_PRIORITY,
        NULL,
        TASK_MOTOR_RUN_CORE);
}

void LinearActuator::stall_guard_task(void *pvParameters)
{
    for (;;)
    {
        for (LinearActuator *la : LinearActuator::all)
        {
            if (la->updateSgTh>0)
            {
                la->driver.SGTHRS(la->updateSgTh/2);
                la->updateSgTh = 0;
            }
            if (la->askForStallGuard)
            {
                la->stallResult = la->driver.diag();
                la->newStallGuardAvailable = true;
            }
        }
        vTaskDelay(TASK_STALLGUARD_DELAY_MS / portTICK_PERIOD_MS);
    }
}

void LinearActuator::motor_run_task(void *pvParameters)
{
    for (;;)
    {
        // Boucle infinie pour l'exécution de votre tâche
        uint64_t t = esp_timer_get_time();
        for (LinearActuator *la : LinearActuator::all)
            if (la->calibrating)
                if(la->calibration(t))
                    la->calibrating = false;

        vTaskDelay(10 / portTICK_PERIOD_MS); // Bad practice, but it's the only way to avoid watchdog reset, solution is to use the FastAccelStepper library
    }
}
void LinearActuator::setup()
{
    motor = engine.stepperConnectToPin(stepPin);
    motor->setDirectionPin(dirPin, !shaft);
    
    all.push_back(this);
    set_max_speed(LINEAR_ACTUATOR_MAX_SPEED);           // set max speed
    set_acceleration(LINEAR_ACTUATOR_MAX_ACCELERATION); // set acceleration
    // motor->enableOutputs();                              // enable motor outputs
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
    // driver.pwm_autoscale(true); // Needed for stealthChop
}

bool LinearActuator::get_stall_result()
{
    askForStallGuard = true;
    if (!newStallGuardAvailable)
        return false;
    else
    {
        newStallGuardAvailable = false;
        return stallResult;
    }
}

void LinearActuator::instant_stop()
{
    motor->forceStop();
}

bool LinearActuator::is_new_acceleration()
{

    if (current_acceleration()!=previousAcceleration)
    {
        previousAcceleration = currentAcceleration;
        return true;
    }
    return false;
}


bool LinearActuator::move_to(float position)
{
    begin_mvt_flag = true;
    motor->moveTo(position * MICRO_STEPS_PER_MM);
    return motor->isRunning() == 0;
}

bool LinearActuator::move(float relativePosition)
{
    begin_mvt_flag = true;
    motor->move(relativePosition * MICRO_STEPS_PER_MM);
    return motor->isRunning() == 0;
}

bool LinearActuator::c_step1()
{
    if (!true)
        return false;
    set_acceleration(LINEAR_ACTUATOR_MAX_SPEED);
    set_max_speed(COARSE_CALIBRATION_SPEED);
    move_left();
    return true;
}

bool LinearActuator::c_step2(int64_t time)
{
    if (!(COARSE_CALIBRATION_SPEED - abs(current_speed()) < 1e-1))
        return false;
    chrono = time;
    updateSgTh = COARSE_CALIBRATION_STALL_VALUE;
    return true;
}

bool LinearActuator::c_step3(int64_t time)
{
    if (!(time - chrono > 30000 && get_stall_result()))
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
    if (!motor->isRunning() == 0)
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
    updateSgTh = FINE_CALIBRATION_STALL_VALUE;
    return true;
}

bool LinearActuator::c_step7(int64_t time)
{
    if (!(time - chrono > 30000 && get_stall_result()))
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
    if (calibrationGoodSamples < FINE_CALIBRATION_SAMPLES)
        return false; // don't forget to implement counter condition in main calibration method
    leftLimit = calibrationFirstWallPosition;
    set_max_speed(COARSE_CALIBRATION_SPEED*3);
    move(-LINEAR_ACTUATOR_MIN_AMPLITUDE);
    return true;
}

bool LinearActuator::c_step9()
{
    if (!motor->isRunning() == 0)
        return false;
    set_max_speed(COARSE_CALIBRATION_SPEED);
    move_right();
    return true;
}

bool LinearActuator::c_step10(int64_t time)
{
    if (!(COARSE_CALIBRATION_SPEED - abs(current_speed()) < 1e-1))
        return false;
    chrono = time;
    updateSgTh = COARSE_CALIBRATION_STALL_VALUE;
    return true;
}

bool LinearActuator::c_step11(int64_t time)
{
    if (!(time - chrono > 30000 && get_stall_result()))
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
    move(FINE_CALIBRATION_WITHDRAWAL_DISTANCE);
    return true;
}

bool LinearActuator::c_step13()
{
    if (!motor->isRunning() == 0)
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
    updateSgTh = FINE_CALIBRATION_STALL_VALUE;
    return true;
}

bool LinearActuator::c_step15(int64_t time)
{
    if (!(time - chrono > 30000 && get_stall_result()))
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

    long rightLimitMicroSteps = floor(rightLimit * MICRO_STEPS_PER_MM);
    long leftLimitMicroSteps = floor(leftLimit * MICRO_STEPS_PER_MM);

    rightLimit = rightLimitMicroSteps / MICRO_STEPS_PER_MM;
    leftLimit = leftLimitMicroSteps / MICRO_STEPS_PER_MM;
    
    motor->setCurrentPosition(-amplitude * MICRO_STEPS_PER_MM / 2);
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
            cal_flag = true;
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
