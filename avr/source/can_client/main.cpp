
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

    msg.id = 97;

    msg.flags.rtr = 0;
    //msg.flags.extended = 0;

    msg.length = 8;
    /*msg.data[0] = 1;
    msg.data[1] = 2;
    msg.data[2] = 3;
    msg.data[3] = 4;
    msg.data[4] = 5;
    msg.data[5] = 6;
    msg.data[6] = 7;
    msg.data[7] = 8;*/
    msg.data[0] = 243;
    msg.data[1] = 'B';
    msg.data[2] = 'C';
    msg.data[3] = 'D';
    msg.data[4] = 'E';
    msg.data[5] = 'F';
    msg.data[6] = 'G';
    msg.data[7] = 'H';

    PORTA |= 0x02;

    while(true) {
        _delay_ms(200);
        if (can_send_message(&msg)) {
            PORTA |= 0xFF;
            _delay_ms(1000);
            PORTA &= ~(0xFF);
            _delay_ms(1000);
            // comment
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
