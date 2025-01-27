/* 
    RaspComManagement is a class that will be used to manage the communication with the Raspberry Pi.
*/

#ifndef RASP_COM_MANAGEMENT_HPP
#define RASP_COM_MANAGEMENT_HPP
#include "vector.hpp"
#include "Player.hpp"

class RaspComManagement
{
  
public:
    RaspComManagement(int baudRate);
    ~RaspComManagement();

    void setup(Vector <Player*> *players);

    static void readWriteDataTask(void *pvParameters);

    void readData();
    void writeData();

private:
    int baudRate;
    Vector <Player*> *players; // Vector of all players
    struct KeyValue
    {
        String key;
        String param;
    };
    Vector<KeyValue*> keyValues;
    void processKeyValues();
};
#endif