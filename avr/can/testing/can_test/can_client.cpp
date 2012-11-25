

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
    can_init(BITRATE_500_KBPS);

    can_t msg;

    //msg.id = 0x0234567C;
    msg.id = 0x0234567F;
    msg.flags.rtr = 0;
    msg.flags.extended = 1;

    msg.length = 8;
    msg.data[0] = 0xde;
    msg.data[1] = 0xad;
    msg.data[2] = 0xbe;
    msg.data[3] = 0xef;
    msg.data[4] = 10;
    msg.data[5] = 11;
    msg.data[6] = 12;
    msg.data[7] = 42;

    while(true) {
        if (can_send_message(&msg)) {
            led(true, false);
            _delay_ms(100);
            led(false, false);
            _delay_ms(100);
        }
        else {
            led(false, true);
            _delay_ms(500);
            led(false, false);
            _delay_ms(500);
        }
        //msg.id --;
    }

    /** ** ++++++++++ ** **/

    return 0;
}

