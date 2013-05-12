
#ifndef _MOTOR_H_
#define _MOTOR_H_

#include <string>
#include "can/bbcan.h"

class Motors {
    public:
        Motors(const std::string _ipcName = "MOTORS", const std::string _canName = "MOTORS");
        ~Motors();

        void setSpeed(short speed0, short speed1, short speed2, short speed3);
        void getSpeed(short& speed0, short& speed1, short& speed2, short& speed3);

    private:
        short* speeds;
        CAN* can;
        std::string ipcName;
        std::string canName;
};

#endif // _MOTOR_H_
