
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>

#include "uart.h"
#include "can.h"

const unsigned int BUTTON_ID [4] = {225, 226, 227, 228};
const unsigned int LED_REMOTE_ID [8] = {229, 230, 231, 232, 233, 234, 235, 236};

const unsigned int LED_REMOTE_PREFIX = 7;

int main () {
    uart_init(57600);
    can_init(BITRATE_100_KBPS);
    sei();

    can_filter_t filter0;
    filter0.id = 0x000;
    filter0.mask = 0x000;
    filter0.flags.rtr = 0;
    can_set_filter(0, &filter0);

    // initialisation of the buttons and leds //
    DDRE &= ~(0xF0);    // buttons as input
    PORTE |= 0xF0;      // activate internal pull-ups
    DDRA |= 0xFF;       // leds as output

    PORTA = 0x01;
    _delay_ms(1000);

    /*while(true) {
        PORTA = 0xFF;
        uart_putc(111);
        _delay_ms(900);
        PORTA = 0x01;
        _delay_ms(100);
    }*/

    /*while(true) {
        if (uart_isnewdata()) {
            uart_putc(uart_getc());
            uart_putc('.');
        }
    }*/

    /*while(true) {
        PORTA = uart_count();
    }*/

    bool button_high = false;

    while(true) {
        _delay_ms(20);
       if (uart_count() >= 12) {  /// neue serial massages vorhanden
            char incomming_serial_data[12];
            memset(incomming_serial_data, 0, 12);

            uart_read(incomming_serial_data, 12);
            //for (int i = 0; i < 12; i++) uart_putc(incomming_serial_data[i]);

            if (incomming_serial_data[0] == 's') {
                //PORTA = 0xFF;
                //_delay_ms(1000);
                //PORTA = 0x01;
                if (((unsigned int)incomming_serial_data[2] >> 5) != LED_REMOTE_PREFIX) {    /// real CAN frame

                    can_t outgoing_can_data;

                    outgoing_can_data.flags.rtr = incomming_serial_data[1];
                    outgoing_can_data.id = incomming_serial_data[2];
                    outgoing_can_data.length = incomming_serial_data[3];
                    for (int i = 0; i < outgoing_can_data.length; i++) {
                        outgoing_can_data.data[i] = incomming_serial_data[4 + i];
                    }
                    can_send_message(&outgoing_can_data);
                }
                else {  /// LED_REMOTE
                    PORTA |= incomming_serial_data[4];
                    PORTA &= ~(incomming_serial_data[5]);
                }
            }
            else if (incomming_serial_data[0] == 'f') {       /// calibrate filters
                can_filter_t filterX;
                filterX.mask = 0x700;
                filterX.mask |= incomming_serial_data[3];
                filterX.id = incomming_serial_data[2];
                filterX.flags.rtr = incomming_serial_data[4];

                can_set_filter(incomming_serial_data[1], &filterX);
            }
        }
        if (can_check_message()) {  /// neue CAN messages vorhanden
            PORTA |= 0xFF;

            char outgoing_serial_data[11];
            can_t incomming_can_data;

            if (can_get_message(&incomming_can_data)) {
                //memset(outgoing_serial_data, 0, 11);
                outgoing_serial_data[0] = incomming_can_data.flags.rtr;
                outgoing_serial_data[1] = incomming_can_data.id;
                outgoing_serial_data[2] = incomming_can_data.length;

                for (int i = 0; i < 8; i++) {
                    outgoing_serial_data[3 + i] = incomming_can_data.data[i];
                }

                for (int i = 0; i < 11; i++) {
                    //uart_putc(*(outgoing_serial_data+i));
                    uart_putc(outgoing_serial_data[i]);
                }
            }
            PORTA &= ~(0xFF);
        }

        if (PINE < 0xF0) {  /// button pressed
            button_high = true;
            // send the signal that a button was pressed ...
            char outgoing_serial_data[11];

            outgoing_serial_data[0] = 0;

            /*unsigned char tmp = (PINE >> 0);
            switch (tmp) {
                case 1:
                    outgoing_serial_data[1] = BUTTON_ID[0];
                    break;
                case 2:
                    outgoing_serial_data[1] = BUTTON_ID[1];
                    break;
                case 4:
                    outgoing_serial_data[1] = BUTTON_ID[2];
                    break;
                case 8:
                    outgoing_serial_data[1] = BUTTON_ID[3];
                    break;
                default:
                    for (int i = 0; tmp & (unsigned char)pow(2, i); i++) outgoing_serial_data[1] = BUTTON_ID[i];
            }*/

            outgoing_serial_data[1] = BUTTON_ID[0];


            outgoing_serial_data[2] = 8;
            outgoing_serial_data[3] = PINE & 0xF0;
            for (int i = 4; i < 8; i++) outgoing_serial_data[i] = 0xFF;

            for (int i = 0; i < 11; i++) {
                uart_putc(outgoing_serial_data[i]);
            }
        }
        else if (button_high) {
            button_high = false;
            char outgoing_serial_data[11];
            outgoing_serial_data[0] = 0;
            outgoing_serial_data[1] = BUTTON_ID[0];
            outgoing_serial_data[2] = 8;
            outgoing_serial_data[3] = 0x00;
            for (int i = 4; i < 8; i++) outgoing_serial_data[i] = 0x00;

            for (int i = 0; i < 11; i++) uart_putc(outgoing_serial_data[i]);
        }
    }
}
