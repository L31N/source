
#include <avr/io.h>
#include <util/delay.h>

#include "can.h"
#include "pwm.h"

int main () {

    const unsigned short MOTOR1 = 0x005;
    const unsigned short MOTOR2 = 0x006;
    const unsigned short DRIBBLER = 0x007;


    can_init(BITRATE_1_MBPS);

    // motor 1
    can_filter_t filter0;
    filter0.id = MOTOR1;
    filter0.mask = 0x7FF;
    filter0.flags.rtr = 0;

    // motor 2
    can_filter_t filter1;
    filter1.id = MOTOR2;
    filter1.mask = 0x7FF;
    filter1.flags.rtr = 0;

    // dribbler
    can_filter_t filter2;
    filter2.id = DRIBBLER;
    filter2.mask = 0x7FF;
    filter2.flags.rtr = 0;

    can_set_filter(0, &filter0);
    can_set_filter(1, &filter1);
    can_set_filter(2, &filter2);

    while(42) {
        if (can_check_message()) {
            can_t message;
            can_get_message(&message);

            // values +/- 255
            short tmp = 0;
            tmp |= (message.data[0] << 8);
            tmp |= (message.data[1] << 0);

            long value = tmp;
            value += 255;
            value *= 1000;
            value /= (2*255);

            if (message.id == MOTOR1) pwm_set((unsigned long)value, 0);
            if (message.id == MOTOR2) pwm_set((unsigned long)value, 1);
            if (message.id == DRIBBLER) pwm_set((unsigned long) value, 2);
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
