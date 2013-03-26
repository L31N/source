
#include <avr/io.h>
#include <util/delay.h>

#include "can.h"

int main () {
    can_init(BITRATE_100_KBPS);

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
        /*if (can_send_message(&msg)) {
            PORTA |= 0xF0;
            _delay_ms(1000);
            PORTA &= ~(0xF0);
            _delay_ms(1000);
        }
        else {
            PORTA |= 0x0F;
            _delay_ms(500);
            PORTA &= ~(0x0F);
            _delay_ms(500);
        }*/
        //_delay_ms(1000);
        //msg.id --;
        can_init(BITRATE_100_KBPS);

    }

    return 0;
}
