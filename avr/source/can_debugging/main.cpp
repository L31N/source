
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "can.h"

int main () {

    // initialisation of the buttons and leds //
    DDRE &= ~(0xF0);    // buttons as input
    PORTE |= 0xF0;      // activate internal pull-ups
    DDRA |= 0xFF;       // leds as output

    /** CAN TESTING HERE **/
    can_init(BITRATE_100_KBPS);
    sei();

    /// DEFINING FILTERS
    //filter 0
    can_filter_t filter0;
    filter0.id = 0x0;
    filter0.mask = 0x0;

    //filter0.id = 0xB30;
    //filter0.mask = 0x000;

    filter0.flags.rtr = 0;
    //filter0.flags.extended = 0;

    // filter 1
    /*can_filter_t filter1;
    filter1.id = 0x02345660;
    filter1.mask = 0x1FFFFFF0;
    filter1.flags.rtr = 0;
    filter1.flags.extended = 0;*/

    // other filters
    //can_filter_t filterX = filter1;

    can_set_filter(0, &filter0);
    /*can_set_filter(1, 0);
    can_set_filter(2, 0);
    can_set_filter(3, 0);
    can_set_filter(4, 0);
    can_set_filter(5, 0);*/

    PORTA = 0xF0;
    _delay_ms(1000);
    PORTA = 0x00;


   while(true) {
        if (can_check_message()) {
            can_t msg;
            PORTA = 0xFF;
            _delay_ms(100);
            PORTA = 0x00;
            if (can_get_message(&msg)) {
                PORTA = 0xFF;
                _delay_ms(500);
                PORTA = 0x00;
                _delay_ms(500);
            }
            else {
                PORTA = 0x01;
                _delay_ms(500);
                PORTA = 0x00;
                _delay_ms(500);
            }
        }

    }
    /** ** ++++++++++ ** **/

    return 0;
}

