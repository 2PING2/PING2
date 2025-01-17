#include "RaspComManagement.hpp"
#include <Arduino.h>
#include "config.h"

RaspComManagement::RaspComManagement(int baudRate)
{
    this->baudRate = baudRate;
}

RaspComManagement::~RaspComManagement()
{
}

void RaspComManagement::setup(Vector<Player *> *players)
{
    Serial.begin(baudRate);
    this->players = players;
}


/* ex of data: 
    "P1/C" to calibrate player 1
    "P1/LA/ML" to move player 1 to the left
    "P1/LA/MR" to move player 1 to the right
    "P1/LA/MT/100" to move player 1 to position 100

*/

void RaspComManagement::readData()
{
    if (Serial.available() > 0)
    {
        String line = Serial.readStringUntil(LINE_SEP);
        String key = line.substring(0, line.indexOf(KEY_SEP));
        // if key begin with PLAYER_KEY

    }
}