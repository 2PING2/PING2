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

void LinearActuator::instant_stop()
{
    motor.setSpeed(0);
    motor.runSpeed();
}

int LinearActuator::run()
{
    return motor.run();
}

