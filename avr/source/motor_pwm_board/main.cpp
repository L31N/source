
#include <avr/io.h>
#include <util/delay.h>

#include "can.h"
#include "pwm.h"

int main () {
    pwm_init();

    const unsigned short BOARD_ID = 1;      /// BOARD_0
    //const unsigned short BOARD_ID = 33;   /// BOARD_1


    can_init(BITRATE_100_KBPS);
    sei();

    // motor 1
    can_filter_t filter0;
    filter0.id = BOARD_ID;
    filter0.mask = 0x7E0;
    filter0.flags.rtr = 0;

    can_set_filter(0, &filter0);

    while(42) {
        if (can_check_message()) {
            can_t message;
            can_get_message(&message);

            // values +/- 255
            short tmp = 0;
            tmp = message.data[2];

            if(message.data[1] == 1)
            {
            	tmp *= -1;
            }

            long value = tmp;
            value += 255;
            value *= 1000;
            value /= (2*255);

            if (message.data[0] == 0) pwm_set((unsigned long)value, 0);
            if (message.data[0] == 1) pwm_set((unsigned long)value, 1);
        }
    }

//    while(42) {
//        can_t data_frame;
//        data_frame.flags.rtr = 0;
//        data_frame.id = 0x000;
//        data_frame.length = 3;
//
//        data_frame.data[0] = PINC;
//        data_frame.data[1] = PINA;
//        data_frame.data[2] = PINE;
//
//        can_send_message (&data_frame);
//
//        _delay_ms(2);
//
//    }
}
