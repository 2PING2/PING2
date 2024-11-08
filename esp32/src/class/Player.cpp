#include "Player.hpp"

Player::Player(int stepPin, int dirPin, int address, int solenoidPin, int beamSwitchRPin) : actuator(stepPin, dirPin, address), solenoid(solenoidPin), beamSwitch(beamSwitchRPin)
{
}

Player::~Player()
{
}

int Player::setup()
{
    beamSwitch.setup();
    actuator.setup();
    return 0;
}

int Player::loop()
{
    beamSwitch.check();
    return 0;
}