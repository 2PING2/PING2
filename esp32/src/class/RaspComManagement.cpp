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
    Serial.print("RaspComManagement setup with players ptr ");
    Serial.println((unsigned long)players);
    Serial.println(players->size());
    
    xTaskCreatePinnedToCore(this->readWriteDataTask,
                            "readWriteDataTask",
                            10000,
                            this,
                            TASK_RASP_COMMUNICATION_PRIORITY,
                            NULL,
                            TASK_RASP_COMMUNICATION_CORE);
}

void RaspComManagement::readWriteDataTask(void *pvParameters)
{
    RaspComManagement *raspComManagement = (RaspComManagement *)pvParameters;
    for (;;)
    {
        raspComManagement->readData();
        raspComManagement->writeData();
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
        const char *line = l_str.c_str();
        // clear keyValues
        for (int i = 0; i < keyValues.size(); i++)
            delete keyValues[i];
            
        keyValues.resize(0);
        bool isParam = false;
        String key = "";
        String param = "";

        const char *c = line - 1;
        do
        {   
            c++;
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
        this->processKeyValues();
    }
}

void RaspComManagement::processKeyValues()
{
    if (keyValues.size() < 2)
        return;


    if (keyValues[0]->key != PLAYER_KEY)
        return;
    

    long playerId = keyValues[0]->param.toInt()-1;

    if (playerId < 0 || playerId >= players->size())
        return;


    Player *player = players->operator[](playerId);
    // ------------ ASK COMMANDS ------------ //
    if (keyValues[1]->key == ASK_CURRENT_POSITION_KEY)
    {
        Serial.println(PLAYER_KEY+PARAM_BEGIN_SEP+String(playerId+1)+PARAM_END_SEP+KEY_SEP+CURRENT_POSITION_KEY+PARAM_BEGIN_SEP+String(player->actuator.current_position())+PARAM_END_SEP);
    }
    if (keyValues[1]->key == ASK_CURRENT_SPEED_KEY)
    {
        Serial.println(PLAYER_KEY+PARAM_BEGIN_SEP+String(playerId+1)+PARAM_END_SEP+KEY_SEP+CURRENT_SPEED_KEY+PARAM_BEGIN_SEP+String(player->actuator.current_speed())+PARAM_END_SEP);
    }
    else if (keyValues[1]->key == ASK_MAX_SPEED_KEY)
    {
        Serial.println(PLAYER_KEY+PARAM_BEGIN_SEP+String(playerId+1)+PARAM_END_SEP+KEY_SEP+MAX_SPEED_KEY+PARAM_BEGIN_SEP+String(player->actuator.max_speed())+PARAM_END_SEP);
    }
    else if (keyValues[1]->key == ASK_CALIBRATED)
    {
        Serial.println(PLAYER_KEY+PARAM_BEGIN_SEP+String(playerId+1)+PARAM_END_SEP+KEY_SEP+CALIBRATION_KEY+PARAM_BEGIN_SEP+String(player->actuator.is_calibrated())+PARAM_END_SEP); 
    }
    else if (keyValues[1]->key == ASK_RIGHT_LIMIT_KEY)
    {
        Serial.println(PLAYER_KEY+PARAM_BEGIN_SEP+String(playerId+1)+PARAM_END_SEP+KEY_SEP+RIGHT_LIMIT_KEY+PARAM_BEGIN_SEP+String(player->actuator.get_right_limit())+PARAM_END_SEP);
    }
    else if (keyValues[1]->key == ASK_LEFT_LIMIT_KEY)
    {
        Serial.println(PLAYER_KEY+PARAM_BEGIN_SEP+String(playerId+1)+PARAM_END_SEP+KEY_SEP+LEFT_LIMIT_KEY+PARAM_BEGIN_SEP+String(player->actuator.get_left_limit())+PARAM_END_SEP);
    }   
    else if (keyValues[1]->key == ASK_SOL_STATE_KEY)
    {
        Serial.println(PLAYER_KEY+PARAM_BEGIN_SEP+String(playerId+1)+PARAM_END_SEP+KEY_SEP+ASK_SOL_STATE_KEY+PARAM_BEGIN_SEP+String(player->solenoid.get_state())+PARAM_END_SEP);
    }
    else if (player->actuator.is_busy())
    {
        Serial.println(PLAYER_KEY+PARAM_BEGIN_SEP+String(playerId+1)+PARAM_END_SEP+KEY_SEP+BUSY_KEY);
    }// ------------ COMMANDS ------------ //
    else if (keyValues[1]->key == CALIBRATION_KEY)
    {
        player->actuator.calibrate();
    }
    else if (keyValues[1]->key == MOVE_TO_LEFT_LIMIT_KEY)
    {
        player->actuator.move_left();
    }
    else if (keyValues[1]->key == MOVE_TO_RIGHT_LIMIT_KEY)
    {
        player->actuator.move_right();
    }
    else if (keyValues[1]->key == MOVE_TO_KEY)
    {
        try {
            player->actuator.move_to(keyValues[1]->param.toFloat());
        } catch (const std::exception& e) {
            Serial.println("Invalid move_to parameter e : " + String(e.what()));
        }
    }
    else if (keyValues[1]->key == SET_MAX_SPEED_KEY)
    {
        try {
            float new_speed = keyValues[1]->param.toFloat();
            player->actuator.set_max_speed(new_speed);
        } catch (const std::exception& e) {
            Serial.println("Invalid set_speed parameter e : " + String(e.what()));
        }
    }
    else if (keyValues[1]->key == SET_MAX_ACCELERATION_KEY)
    {
        try {
            player->actuator.set_acceleration(keyValues[1]->param.toFloat());
        } catch (const std::exception& e) {
            Serial.println("Invalid set_acceleration parameter e : " + String(e.what()));
        }
    }
    else if (keyValues[1]->key == SET_SOL_STATE_KEY)
    {
        try
        {
            bool state = keyValues[1]->param.toInt();
            if (state)
                player->solenoid.activate();
            else
                player->solenoid.deactivate();

        }
        catch(const std::exception& e)
        {
            Serial.println("Invalid solenoid state parameter e : " + String(e.what()));
        }     
    
    }
    else if (keyValues[1]->key == STOP_KEY)
    {
        player->actuator.stop();
    }
    else
    {
        Serial.println("Invalid command");
    }

}

void RaspComManagement::writeData()
{
    for (int i = 0; i < players->size(); i++)
    {
        Player *player = players->operator[](i);
        if (player->actuator.consume_mvt_flag())
        {
            Serial.println(PLAYER_KEY+PARAM_BEGIN_SEP+String(i+1)+PARAM_END_SEP+KEY_SEP+CURRENT_POSITION_KEY+PARAM_BEGIN_SEP+String(player->actuator.current_position())+PARAM_END_SEP);
            Serial.println(PLAYER_KEY+PARAM_BEGIN_SEP+String(i+1)+PARAM_END_SEP+KEY_SEP+CURRENT_SPEED_KEY+PARAM_BEGIN_SEP+String(player->actuator.current_speed())+PARAM_END_SEP);
        }
        if (player->actuator.consume_cal_flag())
        {
            Serial.println(PLAYER_KEY+PARAM_BEGIN_SEP+String(i+1)+PARAM_END_SEP+KEY_SEP+RIGHT_LIMIT_KEY+PARAM_BEGIN_SEP+String(player->actuator.get_right_limit())+PARAM_END_SEP);
            Serial.println(PLAYER_KEY+PARAM_BEGIN_SEP+String(i+1)+PARAM_END_SEP+KEY_SEP+LEFT_LIMIT_KEY+PARAM_BEGIN_SEP+String(player->actuator.get_left_limit())+PARAM_END_SEP);
        }
    }
}