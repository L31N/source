
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "can.h"

int main () {

    // initialisation of the buttons and leds //
    DDRE &= ~(0xF0);    // buttons as input
    PORTE |= 0xF0;      // activate internal pull-ups
    DDRA |= 0xFF;       // leds as output

    can_init(BITRATE_100_KBPS);
    sei();

    can_t msg;

    //msg.id = 0x0234567C;
    //msg.id = 0x0234567F;

    msg.id = 0x1A;

    msg.flags.rtr = 0;
    //msg.flags.extended = 0;

    msg.length = 8;
    msg.data[0] = 'H';
    msg.data[1] = 'e';
    msg.data[2] = 'l';
    msg.data[3] = 'l';
    msg.data[4] = 'o';
    msg.data[5] = '1';
    msg.data[6] = '2';
    msg.data[7] = '3';

    while(true) {
        if (can_send_message(&msg)) {
            PORTA |= 0x10;
            _delay_ms(1000);
            PORTA &= ~(0x10);
            _delay_ms(1000);
        }
        else {
            PORTA |= 0xF0;
            _delay_ms(500);
            PORTA &= ~(0xF0);
            _delay_ms(500);
        }
        //_delay_ms(1000);
        //msg.id --;
    }

    return 0;
}
