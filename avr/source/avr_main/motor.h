
#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "can.h"
#include "uart.h"

class Motor {
    public:
        Motor();
        ~Motor();

        void setSpeed(unsigned char number, signed short speed);
        void test(unsigned char num);

    private:
        unsigned char* ID_MOTOR;
};

#endif // _MOTOR_H_
