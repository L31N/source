
#include <stdlib.h>

#include <util/delay.h>

#include "motor.h"


Motor::Motor() {
    speeds = (signed short*)malloc(4*sizeof(signed short));
    board = (Board*)malloc(sizeof(Board));

    for(int i = 0; i < 4; i++) speeds[i] = 0;
}

Motor::~Motor() {
    free(speeds);
    free(board);
}

void Motor::setSpeed(unsigned char num, signed short speed) {
    uart_init(38400);

    if (num > 3 || speed > 255 || speed < -255) return;

    speeds[num] = speed;

    can_t motor_frame;
    motor_frame.flags.rtr = 0;
    motor_frame.id = MOTOR_CAN_ID;
    motor_frame.length = 8;

    for (int i = 0; i < 4; i++) {
        if (speeds[i] < 0) {
            motor_frame.data[2*i] = 1;
            motor_frame.data[2*i + 1] = speeds[i] * -1;
        }
        else {
            motor_frame.data[2*i] = 0;
            motor_frame.data[2*i + 1] = speeds[i];
        }
    }

    if (!can_send_message(&motor_frame)) {
        /// ERROR WHILE SEND DATA TO MOTOR BOARD
        uart_debug("ERROR WHILE SEND DATA TO MOTOR BOARD");
        board->ledOn(4, true);
        _delay_ms(100);
        board->ledOn(4, false);
    }
}

void Motor::setSpeed(signed short speed0, signed short speed1, signed short speed2, signed short speed3) {
    if (speed0 < -255 || speed0 > 255) return;
    else if (speed1 < -255 || speed1 > 255) return;
    else if (speed2 < -255 || speed2 > 255) return;
    else if (speed3 < -255 || speed3 > 255) return;

    speeds[0] = speed0;
    speeds[1] = speed1;
    speeds[2] = speed2;
    speeds[3] = speed3;

    can_t motor_frame;
    motor_frame.flags.rtr = 0;
    motor_frame.id = MOTOR_CAN_ID;
    motor_frame.length = 8;

    for (int i = 0; i < 4; i++) {
        if (speeds[i] < 0) {
            motor_frame.data[2*i] = 1;
            motor_frame.data[2*i + 1] = speeds[i] * -1;
        }
        else {
            motor_frame.data[2*i] = 0;
            motor_frame.data[2*i + 1] = speeds[i];
        }
    }

    if (!can_send_message(&motor_frame)) {
        /// ERROR WHILE SEND DATA TO MOTOR BOARD
        uart_debug("ERROR WHILE SEND DATA TO MOTOR BOARD");
        board->ledOn(4, true);
        _delay_ms(100);
        board->ledOn(4, false);
    }
}

void Motor::test(unsigned char num) {
    this->setSpeed(num, 70);
    _delay_ms(1000);
    this->setSpeed(num, 0);
    _delay_ms(1000);
    this->setSpeed(num, -70);
    _delay_ms(1000);
    this->setSpeed(num, 0);
    _delay_ms(1000);
}
