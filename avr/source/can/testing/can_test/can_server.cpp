
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "can.h"
#include "can_testing.h"
#include "uart.h"


/*prog_uint8_t can_filter[] = {
    // Group 0
	MCP2515_FILTER(0),				// Filter 0
	MCP2515_FILTER(0),				// Filter 1

	// Group 1
	MCP2515_FILTER_EXTENDED(0),		// Filter 2
	MCP2515_FILTER_EXTENDED(0),		// Filter 3
	MCP2515_FILTER_EXTENDED(0),		// Filter 4
	MCP2515_FILTER_EXTENDED(0),		// Filter 5

	MCP2515_FILTER(0),				// Mask 0 (for group 0)
	MCP2515_FILTER_EXTENDED(0),		// Mask 1 (for group 1)
};*/

/*int main () {
    init_leds();
    uart_init(115200);


    led(true, true);
    _delay_ms(500);
    led(false, false);
    _delay_ms(500);

    for (unsigned long i = 0; i < 100000; i++) {
        if (i%100 == 0) _delay_ms(1);
        if (uart_putc(i%26 + 'a')) led(false, true);
    }

    led(true, true);

    return 0;
}*/


int main () {
    init_leds();
    //uart_init(115200);

    led(true, true);
    _delay_ms(500);
    led(false, false);
    _delay_ms(500);

    /** CAN TESTING HERE **/
    can_init(BITRATE_100_KBPS);

    //can_static_filter(can_filter);

    led(0, 1);
    _delay_ms(500);
    led(0, 0);

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


   while(true) {
        if (can_check_message()) {
            can_t msg;
            led(true, true);
            _delay_ms(100);
            led(false, false);
            if (can_get_message(&msg)) {

                char* data;
                data = (char*) malloc(10);
                memset(data, 0, 10);
                for (int i = 0; i < 8; i++) {
                    data[i] = msg.data[i];
                }
                data[8] = ' ';
                data[9] = '\0';
                uart_putstr(data);

                led(true, true);
                _delay_ms(500);
                led(false, false);
                _delay_ms(500);
            }
            else {
                led(false, true);
                _delay_ms(500);
                led(false, false);
                _delay_ms(500);
            }
        }
        //for (int i = 0; i < 12; i++) uart_putc('X');
        //uart_putc('\n');
        /*uart_putc('H');
        uart_putc('e');
        uart_putc('l');
        uart_putc('l');
        uart_putc('o');
        uart_putc('W');
        uart_putc('o');
        uart_putc('r');
        uart_putc('l');
        uart_putc('d');
        uart_putc('4');
        uart_putc('2');
        _delay_ms(1000);
        PORTB ^= 0x03;*/


        /*char* data = (char*) malloc(2);
        memset (data, 0, 2);
        *data = '.';
        *(data+1) = '\0';
        uart_putstr(data);
        _delay_ms(30);*/

        /*for(int i = 0; i < 8; i++) {
            uart_putc(65+i);
        }*/
        //_delay_ms(2000);
        /*if (uart_count() >= 12) {
            led(true, true);
            _delay_ms(1000);
            led(false, false);
            char buffer[12];
            uart_read(buffer, 12);
        }*/

    }



    /** ** ++++++++++ ** **/

    return 0;
}

