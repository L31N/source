
#include <stdlib.h>

#include <util/delay.h>

#include "motor.h"


Motor::Motor() {
    //ID_MOTOR = new unsigned char[4];
    ID_MOTOR = (unsigned char*)malloc(4*sizeof(unsigned char));

    ID_MOTOR[0] = 1;
    ID_MOTOR[1] = 2;
    ID_MOTOR[2] = 33;
    ID_MOTOR[3] = 34;
}

Motor::~Motor() {
    free(ID_MOTOR);
}

void Motor::setSpeed(unsigned char num, signed short speed) {
    if (num > 3 || speed > 255 || speed < -255) return;

    can_t motor_frame;
    motor_frame.flags.rtr = 0;
    motor_frame.id = ID_MOTOR[num];
    motor_frame.length = 8;

    motor_frame.data[0] = num%2;
    if (speed < 0) {
        motor_frame.data[1] = 1;
        motor_frame.data[2] = -speed;
    }
    else {
        motor_frame.data[1] = 0;
        motor_frame.data[2] = speed;
    }

    for (int i = 3; i < 8; i++) motor_frame.data[i] = 0xFF;

    can_send_message(&motor_frame);
}

void Motor::test(unsigned char num) {
    this->setSpeed(num, 255);
    _delay_ms(2000);
    this->setSpeed(num, 0);
    _delay_ms(1000);
    this->setSpeed(num, -255);
    _delay_ms(2000);
    this->setSpeed(num, 0);
    _delay_ms(1000);
}
