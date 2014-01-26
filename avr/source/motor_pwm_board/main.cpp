
#include <avr/io.h>
#include <util/delay.h>

#include <math.h>

#include "can.h"
#include "pwm.h"

#define MIN_OFFSET 45

//const bool BOARD_0 = true;      /// board0
const bool BOARD_0 = false;     /// board1

const unsigned short BOARD_ID = 9;

struct pwm_time {
    unsigned char ontime;
    unsigned char offtime;
};

struct pwm_time pwm_lookup[13] {
    {2, 5},
    {2, 4},
    {2, 3},
    {3, 4},
    {2, 2},
    {4, 3},
    {3, 2},
    {2, 1},
    {5, 2},
    {3, 1},
    {4, 1},
    {5, 1},
    {6, 1}
};

int main () {

    DDRA = 0xFF;

    pwm_init();

    /** testing ++++++++++++++++++ **/

    /*_delay_ms(2000);

    while(true) {
        signed short value0 = 100;
        long lvalue0 = value0;

        lvalue0 += 255;
        lvalue0 *= 1000;
        lvalue0 /= (2*255);

        pwm_set(1000, 0);
        pwm_set(1000, 1);
        _delay_ms(60);

        pwm_set((unsigned long)lvalue0, 0);
        pwm_set((unsigned long)lvalue0, 1);

        _delay_ms(1000);

        pwm_set(500, 0);
        pwm_set(500, 1);

        _delay_ms(1000);

    }*/

  /*  long lvalue0 = -45;
    long lvalue1 = -45;

    lvalue0 += 255;
    lvalue0 *= 1000;
    lvalue0 /= (2*255);

    lvalue1 += 255;
    lvalue1 *= 1000;
    lvalue1 /= (2*255);

    pwm_set(500, 0);
    pwm_set(500, 1);

    _delay_ms(3000);

    pwm_set_ontime(1, 0);
    pwm_set_offtime(2, 0);
    pwm_set_ontime(1, 1);
    pwm_set_offtime(2, 1);

    pwm_set(1000, 0);
    pwm_set(1000, 1);

    while(true) {
        //pwm_set((unsigned long)lvalue0, 0);
        //pwm_set((unsigned long)lvalue1, 1);
        pwm_set((unsigned long)lvalue0, 0);
        pwm_set((unsigned long)lvalue1, 1);
        _delay_ms(20);
        pwm_set((unsigned long)500, 0);
        //pwm_set((unsigned long)500, 1);
        _delay_ms(10);
    }*/

    /** ---------------------------- **/

    can_init(BITRATE_1_MBPS);
    sei();

    // motor 1
    can_filter_t filter0;
    filter0.id = BOARD_ID;
    filter0.mask = 0x7FF;
    filter0.flags.rtr = 0;

    can_set_filter(0, &filter0);

    while(42) {

        if (can_check_message()) {

            //PORTA ^= 0xF0;

            can_t message;
            can_get_message(&message);

            signed short value0;
            signed short value1;

            if (BOARD_0) {
                value0 = message.data[1];
                value1 = -message.data[3];

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


            /* pulsing values */
            if (value0 < 45) {
                if (value0 < 5) {
                    pwm_set(500, 0);
                }
                else if (value0 < 13) {
                    pwm_set_ontime(pwm_lookup[0].ontime, 0);
                    pwm_set_offtime(pwm_lookup[0].offtime, 0);
                    pwm_set(588, 0);    // speed 45
                }
                else {
                    double dindex = 0.44*value0-4.9;
                    unsigned char cindex = (unsigned char)round(dindex)%13;
                    pwm_set_ontime(pwm_lookup[cindex].ontime, 0);
                    pwm_set_offtime(pwm_lookup[cindex].offtime, 0);
                    pwm_set(588, 0);    // speed 45
                }
            }
            else {
                pwm_set_ontime(1, 0);
                pwm_set_offtime(0, 0);
                pwm_set((unsigned long)lvalue0, 0);
            }

            if (value1 < 45) {
                if (value1 < 5) {
                    pwm_set(500, 1);
                }
                else if (value1 < 13) {
                    pwm_set_ontime(pwm_lookup[0].ontime, 1);
                    pwm_set_offtime(pwm_lookup[0].offtime, 1);
                    pwm_set(588, 1);    // speed 45
                }
                else {
                    double dindex = 0.44*value1-4.9;
                    unsigned char cindex = (unsigned char)round(dindex)%13;
                    pwm_set_ontime(pwm_lookup[cindex].ontime, 1);
                    pwm_set_offtime(pwm_lookup[cindex].offtime, 1);
                    pwm_set(588, 1);    // speed 45
                }
            }
            else {
                pwm_set_ontime(1, 1);
                pwm_set_offtime(0, 1);
                pwm_set((unsigned long)lvalue0, 1);
            }



            pwm_set((unsigned long)lvalue0, 0);
            pwm_set((unsigned long)lvalue1, 1);
        }
    }
}
