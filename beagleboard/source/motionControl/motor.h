
#ifndef _MOTOR_H_
#define _MOTOR_H_

#include <string>
#include "bbcan.h"

class Motor {
    public:
        Motor(const std::string _ipcName, const std::string _canMember);
        ~Motor();

        void setSpeed(short _speed);
        short getSpeed();

    private:
        short speed;
        CAN* can;
        std::string ipcName;
        std::string canMember;
};

#endif // _MOTOR_H_
