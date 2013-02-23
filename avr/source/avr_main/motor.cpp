
#include <stdlib.h>

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

    uart_debug("\n\rid: %d data[0]: %d data[1]: %d data[2]: %d data[3]: %d data[4]: %d data[5]: %d data[6]: %d data[7]: %d ", motor_frame.id,
                                                                                                                                motor_frame.data[0],
                                                                                                                                motor_frame.data[1],
                                                                                                                                motor_frame.data[2],
                                                                                                                                motor_frame.data[3],
                                                                                                                                motor_frame.data[4],
                                                                                                                                motor_frame.data[5],
                                                                                                                                motor_frame.data[6],
                                                                                                                                motor_frame.data[7]);
    can_send_message(&motor_frame);
}
