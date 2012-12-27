

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "can.h"
#include "can_testing.h"

int main () {
    init_leds();

    led(true, true);
    _delay_ms(500);
    led(true, true);
    _delay_ms(500);

    /** CAN TESTING HERE **/
    can_init(BITRATE_1_MBPS);

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
    //msg.data[8] = '4';
    //msg.data[9] = '5';

    while(true) {
        if (can_send_message(&msg)) {
            led(true, false);
            _delay_ms(1000);
            led(false, false);
            _delay_ms(1000);
        }
        else {
            led(false, true);
            _delay_ms(500);
            led(false, false);
            _delay_ms(500);
        }
        //_delay_ms(1000);
        //msg.id --;
    }

    /** ** ++++++++++ ** **/

    return 0;
}

