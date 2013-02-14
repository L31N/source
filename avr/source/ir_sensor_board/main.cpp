
#include <avr/io.h>
#include <util/delay.h>

#include "can.h"

int main () {
    can_init(BITRATE_1_MBPS);

    can_filter_t filter0;
    filter0.id = 0x00F;
    filter0.mask = 0x7FF;

    filter0.flags.rtr = 0;

    can_set_filter(0, &filter0);

    DDRC = 0x00;
    DDRA = 0x00;
    DDRE = 0x00;

    while(42) {
        can_t data_frame;
        data_frame.flags.rtr = 0;
        data_frame.id = 0x000;
        data_frame.length = 3;

        data_frame.data[0] = PINC;
        data_frame.data[1] = PINA;
        data_frame.data[2] = PINE;

        can_send_message (&data_frame);

        _delay_ms(2);

    }
}
