#include "LinearActuator.hpp"

Vector<LinearActuator *> LinearActuator::all;

void LinearActuator::setup()
{
    all.push_back(this);
    set_max_speed(LINEAR_ACTUATOR_MAX_SPEED);            // set max speed
    set_acceleration(LINEAR_ACTUATOR_MAX_ACCELERATION); // set acceleration
    motor.enableOutputs();                             // enable motor outputs
    driver.begin();
    driver.senddelay(8); // not sure about this
    driver.toff(4);
    driver.blank_time(24);
    driver.rms_current(TMC_MAX_CURRENT);
    driver.mres(8-MICROSTEP_POWER_OF_2);
    driver.TCOOLTHRS(0xFFFFF); // 20bit max
    driver.semin(5);
    driver.semax(2);
    driver.shaft(shaft);
    driver.pwm_autoscale(true);     // Needed for stealthChop
}

int LinearActuator::calibrate_right()
{
    set_max_speed(COARSE_CALIBRATION_SPEED);
    move_right();
    // driver.VACTUAL(-COARSE_CALIBRATION_SPEED*MICRO_STEPS_PER_MM);
    if (abs(abs(current_speed())-COARSE_CALIBRATION_SPEED)>1)
        return -1;
    int64_t t = esp_timer_get_time();
    if (chrono == 0)
        chrono = t;
    if (t-chrono<20000)
        return -2;
    if (driver.SG_RESULT() > COARSE_CALIBRATION_STALL_VALUE)
        return 0; // no collision
    instant_stop();
    chrono = 0;
    rightLimit = current_position();
    if (is_left_calibrated())
    {
        float _amplitude = amplitude();
        rightLimit = -_amplitude / 2;
        leftLimit = _amplitude / 2;
        motor.setCurrentPosition(-amplitude() * MICRO_STEPS_PER_MM / 2);
    }
    set_max_speed(LINEAR_ACTUATOR_MAX_SPEED);
    return 1;
}

int LinearActuator::calibrate_left()
{
    set_max_speed(COARSE_CALIBRATION_SPEED);
    move_left();
    // driver.VACTUAL(COARSE_CALIBRATION_SPEED*MICRO_STEPS_PER_MM);

    if (abs(abs(current_speed())-COARSE_CALIBRATION_SPEED)>1)
        return -1;
    int64_t t = esp_timer_get_time();
    if (chrono == 0)
        chrono = t;
    if (t-chrono<20000)
        return -2;

    if (driver.SG_RESULT() > COARSE_CALIBRATION_STALL_VALUE)
        return 0;

    instant_stop();
    chrono = 0;

    leftLimit = current_position();
    // if (is_right_calibrated())
    //     motor.setCurrentPosition(amplitude() * MICRO_STEPS_PER_MM / 2);
    set_max_speed(LINEAR_ACTUATOR_MAX_SPEED);
    return 1;
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

