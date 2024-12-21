#include "Player.hpp"

Player::Player(int stepPin, int dirPin, TMC2209::SerialAddress address, int solenoidPin, int beamSwitchRPin) : actuator(stepPin, dirPin, address), solenoid(solenoidPin), beamSwitch(beamSwitchRPin)
{
}

Player::~Player()
{
}

int Player::setup()
{
    actuator.setup();
    solenoid.setup();
    return 0;
}

int Player::loop()
{
    return 0;
}