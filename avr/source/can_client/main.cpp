
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "can.h"

int main () {

    // initialisation of the buttons and leds //
    DDRE &= ~(0xF0);    // buttons as input
    PORTE |= 0xF0;      // activate internal pull-ups
    DDRA |= 0xFF;       // leds as output

    can_init(BITRATE_1_MBPS);
    sei();

    can_t msg;
    can_t msg1;

    //msg.id = 0x0234567C;
    //msg.id = 0x0234567F;

    msg.id = 69;
    msg1.id = 42;

    msg.flags.rtr = 0;
    msg1.flags.rtr = 0;
    //msg.flags.extended = 0;

    msg.length = 8;
    msg1.length = 4;
    /*msg.data[0] = 1;
    msg.data[1] = 2;
    msg.data[2] = 3;
    msg.data[3] = 4;
    msg.data[4] = 5;
    msg.data[5] = 6;
    msg.data[6] = 7;
    msg.data[7] = 8;*/
    msg.data[0] = 0b11111111;
    msg.data[1] = 'B';
    msg.data[2] = 'C';
    msg.data[3] = 'D';
    msg.data[4] = 'E';
    msg.data[5] = 'F';
    msg.data[6] = 'G';
    msg.data[7] = 'H';

    msg1.data[0] = 42;
    msg1.data[1] = 21;
    msg1.data[2] = 6;
    msg1.data[3] = 63;

    //PORTA |= 0x02;

    unsigned char i = 0;
    while(true) {
        i++;
        //_delay_ms(200);
        if (can_send_message(&msg) && can_send_message(&msg1)) {
            PORTA = 0x01;
            //PORTA |= 0xFF;
            //_delay_ms(1000);
            //PORTA &= ~(0xFF);
            //_delay_ms(1000);
            // comment
        }
        else {
            PORTA ^= 0xF0;
            //PORTA |= 0xF0;
            //_delay_ms(500);
            //PORTA &= ~(0xF0);
            //_delay_ms(500);
        }
        //_delay_ms(1000);
        //msg.id --;
        _delay_ms(500);
        msg.data[7] = i;
        msg1.data[3] = i;
        //if (i%10 == 0) _delay_ms(100);
    }

    return 0;
}
