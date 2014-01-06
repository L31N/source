
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "can.h"
#include "uart.h"

int main () {

    uart_init(38400);

    // initialisation of the buttons and leds //
    DDRE &= ~(0xF0);    // buttons as input
    PORTE |= 0xF0;      // activate internal pull-ups
    DDRA |= 0xFF;       // leds as output

    /** CAN TESTING HERE **/
    can_init(BITRATE_100_KBPS);
    sei();

    // motor 1
    can_filter_t filter0;
    filter0.id = 9;
    //filter0.mask = 0x7FF;
    filter0.mask = 0;
    filter0.flags.rtr = 0;

    can_set_filter(0, &filter0);

    PORTA = 0x01;
    _delay_ms(1000);
    PORTA = 0x02;

    int j = 0;

    while(true) {
        can_t frame;
        frame.id = 0x42;
        frame.flags.rtr = 0;
        frame.length = 8;
        for(int i = 0; i < 8; i++) {
            frame.data[i] = j + i;
        }
        j++;

        can_send_message(&frame);

        //_delay_ms(2);
        _delay_us(10);

        /*if (j > 4) {
            j = 0;
            _delay_ms(200);
        }*/
    }


   while(true) {
        if (can_check_message()) {
            PORTA ^= 0x03;

            /*can_t msg;
            can_get_message(&msg);*/

            can_t message;
            can_get_message(&message);
            for (int i = 0; i < 8; i++) {
                uart_debug("%d: %d\n\r", i, message.data[i]);
                _delay_ms(5);
            }
            uart_debug("##########################\n\n\r");
        }
        if (PINE < 0xF0) {
            can_t frame;
            frame.id = 0x09;
            frame.flags.rtr = 0;
            frame.length = 2;
            frame.data[0] = 0x2A;
            frame.data[1] = 0x06;

            if (can_send_message(&frame)) {
                PORTA &= ~0xE0;
                PORTA ^= 0x10;
            }
            else PORTA = 0xE0;

            uart_debug(".\n\r");
            _delay_ms(500);
        }
    }


    /** ** ++++++++++ ** **/

    return 0;
}

