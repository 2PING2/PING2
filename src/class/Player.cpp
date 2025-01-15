#include "Player.hpp"

Vector <Player*> Player::all;

Player::Player(int stepPin, int dirPin, uint8_t address, bool motorShaft, int solenoidPin, int beamSwitchRPin) : actuator(stepPin, dirPin, address, motorShaft), solenoid(solenoidPin), beamSwitch(beamSwitchRPin)
{
}

Player::~Player()
{
}

int Player::setup()
{
    actuator.setup();
    solenoid.setup();
    beamSwitch.setup();
    all.push_back(this);
    return 0;
}

int Player::loop()
{
    return 0;
}