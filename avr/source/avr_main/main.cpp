
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <math.h>

#include "board.h"
#include "motor.h"
#include "can.h"

/*const unsigned char id_ir_front = 129;
const unsigned char id_ir_right = 135;
const unsigned char id_ir_back = 144;
const unsigned char id_ir_left = 150;

void init_avr();
bool button(unsigned char num = 5);     // 5 is anybutton
void set_ir_can_filter();
void motor(unsigned short num, signed short speed);
bool get_ir_value(unsigned char num);*/
//unsigned short get_ball_dir();


int main () {

    uart_init(38400);
    can_init(BITRATE_100_KBPS);
    sei();

    Board board;
    Motor motor;
    board.ledOn(7,true);

    while(!board.buttonDown());
    board.ledOn(2,true);

    motor.setSpeed(0, 100);
    motor.setSpeed(1, 100);
    motor.setSpeed(2, -100);
    motor.setSpeed(3, -100);

    _delay_ms(3000);

    motor.setSpeed(0, 0);
    motor.setSpeed(1, 0);
    motor.setSpeed(2, 0);
    motor.setSpeed(3, 0);

    while(1);

    board.ledOn(2, 1);

    while(true) {

        /*for (int i = 0; i < 4; i++) {
            board.ledOn(i, board.buttonDown(i));
            motor.setSpeed(i, board.buttonDown(i)*-200);
            _delay_ms(100);
        }*/



        /*if(board.buttonDown(0))
        {
            board.ledOn(0, true);
            board.ledOn(7, false);
            board.ledOn(4, false);
            motor.setSpeed(2, 255);
        }
        if(board.buttonDown(1))
        {
            board.ledOn(7, true);
            board.ledOn(0, false);
            board.ledOn(4, false);
            motor.setSpeed(2, 0);
        }
        if(board.buttonDown(3))
        {
            board.ledOn(7, false);
            board.ledOn(0, false);
            board.ledOn(4, true);
            motor.setSpeed(2, -255);
        }*/

        _delay_ms(200);
    }

    while(true);

    /*while(true) {
        for (unsigned char i = 0; i < 4; i++) {
            board.ledOn(i, board.buttonDown(i));
            motor.setSpeed(i, 100 * board.buttonDown(i));
        }
    }*/
}

    /// ++++++++++++++++++++++++ ///

    /*init_avr();
    can_init(BITRATE_100_KBPS);
    sei();

    set_ir_can_filter();

    PORTA = 0x01;

    while(!button());

    PORTA = 0x02;

    while(42) {
        PORTA = (get_ir_value(0));
        _delay_ms(10);
    }
}

void init_avr() {
    DDRA = 0xFF;        // leds
    DDRE &= ~(0xF0);       // buttons
    PORTE |= 0xF0;
}

bool button(unsigned char num) {
    if (num == 5) {
        if (PINE < 0xF0) return true;
        else return false;
    }
    else {
        if (PINE & (1 << (4 + num))) return true;
        else return false;
    }
}


void set_ir_can_filter() {
    can_filter_t filter_ir;
    filter_ir.flags.rtr = 0;
    filter_ir.id = 0x01;
    filter_ir.mask = 0x00;

    can_set_filter(0, &filter_ir);
}

void motor(unsigned short num, signed short speed) {
    if (num > 3) {
        return;
    }
    else {
        can_t motor_frame;
        motor_frame.flags.rtr = 0;

        if (num <= 1) motor_frame.id = 1 + num;
        else motor_frame.id = 33 + num - 2;

        motor_frame.id = 1 + num;
        motor_frame.length = 8;

        motor_frame.data[0] = num%2;
        if (speed < 0) {
            motor_frame.data[1] = 0;
            motor_frame.data[2] = -speed;
        }
        else {
            motor_frame.data[1] = 1;
            motor_frame.data[2] = speed;
        }

        can_send_message(&motor_frame);
    }
}

bool get_ir_value(unsigned char num) {
    unsigned long value = 0;
    if (can_check_message()) {
        can_t ir_data;
        can_get_message(&ir_data);
        value = (unsigned long)ir_data.data[0];
        value |= ((unsigned long)ir_data.data[1] << 8);
        value |= ((unsigned long)ir_data.data[2] << 16);
    }

    return ~(value) & (1 << num);
}*/

//unsigned short get_ball_dir() {}
