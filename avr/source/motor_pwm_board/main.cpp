
#include <avr/io.h>
#include <util/delay.h>

#include "can.h"
#include "pwm.h"

int main () {

    DDRA = 0xFF;

    pwm_init();

    const bool BOARD_0 = true;      /// board0
    //const bool BOARD_0 = false;     /// board1

    unsigned short BOARD_ID = 1;

    can_init(BITRATE_100_KBPS);
    sei();

    // motor 1
    can_filter_t filter0;
    filter0.id = BOARD_ID;
    filter0.mask = 0x7FF;
    filter0.flags.rtr = 0;

    can_set_filter(0, &filter0);

    while(42) {
        if (can_check_message()) {

            PORTA ^= 0xF0;

            can_t message;
            can_get_message(&message);

            signed short value0;
            signed short value1;

            if (BOARD_0) {
                value0 = message.data[1];
                value1 = message.data[3];

                if (message.data[0] == 1) value0 *= -1;
                if (message.data[2] == 1) value1 *= -1;
            }
            else {
                //value0 = message.data[5] *= -1;
                //value1 = message.data[7] *= -1;

                value0 = -message.data[5];
                value1 = -message.data[7];

                if (message.data[4] == 1) value0 *= -1;
                if (message.data[6] == 1) value1 *= -1;
            }

            long lvalue0 = value0;
            long lvalue1 = value1;

            lvalue0 += 255;
            lvalue0 *= 1000;
            lvalue0 /= (2*255);

            lvalue1 += 255;
            lvalue1 *= 1000;
            lvalue1 /= (2*255);


            pwm_set((unsigned long)lvalue0, 0);
            pwm_set((unsigned long)lvalue1, 1);
        }
    }
}
