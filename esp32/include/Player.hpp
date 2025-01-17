#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "LinearActuator.hpp"
#include "Solenoid.hpp"
#include "BeamSwitch.hpp"
#include "vector.hpp"

class Player
{
public:
    Player(int stepPin, int dirPin, uint8_t address,bool motorShaft, int solenoidPin, int beamSwitchRPin);
    int setup();
    int loop();
    ~Player();

    LinearActuator actuator;
    Solenoid solenoid;
    BeamSwitch beamSwitch;

public :
    static Vector<Player*> all;
};

#endif