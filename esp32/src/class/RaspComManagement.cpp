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


void RaspComManagement::readData()
{
    if (Serial.available() > 0)
    {
        String line = Serial.readStringUntil(LINE_SEP);
    }
}