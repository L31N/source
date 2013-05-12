
#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "can.h"
#include "uart.h"
#include "board.h"

class Motor {
    public:
        Motor();
        ~Motor();

        void setSpeed(unsigned char number, signed short speed);
        void setSpeed(signed short speed0, signed short speed1, signed short speed2, signed short speed3);
        void test(unsigned char num);

    private:
        static const unsigned char MOTOR_CAN_ID = 9;
        signed short* speeds;
        Board* board;
};

#endif // _MOTOR_H_
