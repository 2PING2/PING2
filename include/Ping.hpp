/*
PING is the main class of the project. It is responsible for the initialization of the game and the management of the game loop.
Everything may be static, as there is only one instance of the game.
*/

#ifndef PING_HPP
#define PING_HPP

#include "Player.hpp"
#include "config.h"
#include "RaspComManagement.hpp"

class PING
{
private:
    PING() {};

public:
    ~PING() {};
    static void setup();

#ifndef EVERYTHING_PUBLIC
private:
#endif
    static Player player1, player2, player3, player4;
    static RaspComManagement raspComManager;
    static TaskHandle_t solenoidOvertempTaskHandle;
    static void solenoid_overtemp_task(void *pvParameters);
};

#endif