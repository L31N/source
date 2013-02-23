
#include <avr/io.h>
#include <util/delay.h>

#include "can.h"
#include "pwm.h"

int main () {

    pwm_init();
    //pwm_set(500, 0);
    //_delay_ms(1000);
    /*pwm_set(1000, 0);
    _delay_ms(1000);
    pwm_set(500, 0);
    _delay_ms(1000);
    pwm_set(300, 0);*/
    //pwm_set(800, 0);
    //_delay_ms(1000);
    pwm_set(500, 0);
    _delay_ms(10000);

    /*for (int i = 500; i < 1000; i++) {
        pwm_set(i, 0);
        _delay_ms(10);
    }*/

    pwm_set(550, 0);        /// 555 is minimum value
    //_delay_ms(500);
    //pwm_set(1000, 0);

    /*for (int i = 600; i > 500; i-=5) {
        pwm_set(i, 0);
        _delay_ms(2000);
    }*/

    //_delay_ms(2000);
    //pwm_set (545, 0);

    /*pwm_set(700, 0);
    _delay_ms(1000);
    pwm_set(999, 0);*/
    //_delay_ms(5000);
    //pwm_set(500, 0);

    while(1);

    const unsigned short MOTOR1 = 1;
//    const unsigned short MOTOR2 = 2;
//    const unsigned short MOTOR1 = 33;
//    const unsigned short MOTOR2 = 34;


    can_init(BITRATE_100_KBPS);

    // motor 1
    can_filter_t filter0;
    filter0.id = MOTOR1;
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
