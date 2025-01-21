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
    
    xTaskCreatePinnedToCore(this->readDataTask,
                            "readDataTask",
                            10000,
                            this,
                            TASK_RASP_COMMUNICATION_PRIORITY,
                            NULL,
                            TASK_RASP_COMMUNICATION_CORE);
}

void RaspComManagement::readDataTask(void *pvParameters)
{
    RaspComManagement *raspComManagement = (RaspComManagement *)pvParameters;
    for (;;)
    {
        raspComManagement->readData();
        vTaskDelay(TASK_RASP_COMMUNICATION_DELAY_MS / portTICK_PERIOD_MS);
    }
}

/* ex of data we can receive from the Raspberry Pi:
    "P{1}/C" to calibrate player 1
    "P{1}/ML" to move player 1 to the left
    "P{2}/MR" to move player 2 to the right
    "P{1}/MT{100}" to move player 1 to position 100
    "P{1}/S{100}" to set the speed of player 1 in mm/s
    "P{1}/A{100" to set the acceleration of player 1 in mm/s^2
    "P{1}/SOL_ON" to turn on the solenoid of player 1
    "P{1}/SOL_OFF" to turn off the solenoid of player 1
*/

void RaspComManagement::readData()
{
    if (Serial.available() > 0)
    {
        // const char *line = Serial.readStringUntil(LINE_SEP).c_str();
        String l_str = Serial.readStringUntil(LINE_SEP);
        Serial.println("Received: " + l_str);
        const char *line = l_str.c_str();
        // clear keyValues
        for (int i = 0; i < keyValues.size(); i++)
            delete keyValues[i];
            
        keyValues.resize(0);
        Serial.println("flag1");
        bool isParam = false;
        String key = "";
        String param = "";

        const char *c = line - 1;
        do
        {   
            c++;
            Serial.println("flag1."+String(*c));
            if (*c == KEY_SEP || *c == '\0')
            {
                KeyValue * kv = new KeyValue();
                kv->key = key;
                kv->param = param;
                keyValues.push_back(kv);
                key = "";
                param = "";
            }
            else if (*c == PARAM_BEGIN_SEP)
                isParam = true;
            else if (*c == PARAM_END_SEP)
                isParam = false;
            else if (isParam)
                param += *c;
            else
                key += *c;
        } while (*c != '\0');
        Serial.println("flag2");
        this->processKeyValues();
    }
}

void RaspComManagement::processKeyValues()
{
    Serial.println("Processing key values");
    if (keyValues.size() < 2)
        return;

    Serial.println("flag3");

    if (keyValues[0]->key != PLAYER_KEY)
        return;
    
    Serial.println("flag4");

    long playerId = keyValues[0]->param.toInt();

    Serial.println("flag5 with player Id " + String(playerId));
    if (playerId < 0 || playerId >= players->size())
        return;

    Serial.println("flag6");

    Player *player = players->operator[](playerId);

    Serial.println("flag7");

    Serial.println("flag7.1");

    if (keyValues[1]->key == CALIBRATE_KEY)
    {
        Serial.println("cal key");
        player->actuator.calibrate();
        Serial.println("Calibrating player " + String(playerId));
    }
    else if (keyValues[1]->key == MOVE_LEFT_KEY)
    {
        Serial.println("move left key");
        player->actuator.move_left();
    }
    else if (keyValues[1]->key == MOVE_RIGHT_KEY)
    {
        Serial.println("move right key");
        player->actuator.move_right();
    }
    else if (keyValues[1]->key == MOVE_TO_KEY)
    {
        Serial.println("move to key");
        player->actuator.move_to(keyValues[1]->param.toFloat());
    }
    else if (keyValues[1]->key == SET_SPEED_KEY)
    {
        Serial.println("set speed key");
        player->actuator.set_speed(keyValues[1]->param.toFloat());
    }
    else if (keyValues[1]->key == SET_ACCELERATION_KEY)
    {
        Serial.println("set acceleration key");
        player->actuator.set_acceleration(keyValues[1]->param.toFloat());
    }
    else if (keyValues[1]->key == SOL_ON_KEY)
    {
        Serial.println("sol on key");
        player->solenoid.activate();
    }
    else if (keyValues[1]->key == SOL_OFF_KEY)
    {
        Serial.println("sol off key");
        player->solenoid.deactivate();
    }
    else
    {
        Serial.println("Invalid command");
    }

    Serial.println("flag8");
}