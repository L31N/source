
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "can.h"

void init_avr();
bool button(unsigned char num = 5);     // 5 is anybutton
void set_ir_can_filter();
void motor(unsigned short num, signed short speed);

int main () {
    avr_init();
    can_init(BITRATE_100_KBPS);

    set_ir_can_filter();

    while(!button());

    while(42) {
        if (button()) {
            motor(0, 50);
            _delay_ms(5000);
            motor(0, 0);
        }
    }
}

void init_avr() {
    DDRA = 0xFF;        // leds
    DDRE &= ~(0xF0);       // buttons
}

bool button(unsigned char num) {

}


void set_ir_can_filter() {
    can_filter_t filter_ir;
    filter_ir.rtr = 0;
    filter_ir.id = 0x01;
    filter_ir.mask = 0x00;

    can_set_filter(&filter_ir);
}

void motor(unsigned short num, signed short speed) {
    if (motor > 3) {
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
        if (speed < 0) motor_frame.data[1] = 0;
        else motor_frame.data[1] = 1;
        motor_frame.data[2] = abs(speed);

        can_send_message(&motor_frame);
    }
}
