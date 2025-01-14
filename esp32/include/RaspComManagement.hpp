/* 
    RaspComManagement is a class that will be used to manage the communication with the Raspberry Pi.
*/

#ifndef RASP_COM_MANAGEMENT_HPP
#define RASP_COM_MANAGEMENT_HPP

class RaspComManagement
{
  
public:
    RaspComManagement(int baudRate);
    ~RaspComManagement();

    void setup();
};
#endif