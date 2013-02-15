
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>

#include "uart.h"
#include "can.h"
#include "can_testing.h"

const unsigned int BUTTON_PRESSED_ID = 2;
const unsigned int LED_REMOTE_ID = 20;

int main () {
    uart_init(115200);
    init_leds();
    can_init(BITRATE_1_MBPS);

    can_filter_t filter0;
    filter0.id = 0x000;
    filter0.mask = 0x700;
    filter0.flags.rtr = 0;
    can_set_filter(0, &filter0);

    // initialisation of the buttons and leds //
    DDRE &= ~(0xF0);    // buttons as input
    DDRA |= 0xFF;       // leds as output

    bool f_pressed = false;

    indicate_init();

    while(true) {
       if (uart_count() >= 12) {  /// neue serial massages vorhanden
            char incomming_serial_data[12];
            memset(incomming_serial_data, 0, 12);

            uart_read(incomming_serial_data, 12);
            for (int i = 0; i < 12; i++) uart_putc(incomming_serial_data[i]);

            led(false, true);

            if (incomming_serial_data[0] == 's') {
                led(true, false);

                if (incomming_serial_data[2] != LED_REMOTE_ID) {    /// real CAN frame
                    can_t outgoing_can_data;

                    outgoing_can_data.flags.rtr = incomming_serial_data[1];
                    outgoing_can_data.id = incomming_serial_data[2];
                    outgoing_can_data.length = incomming_serial_data[3];
                    for (int i = 0; i < outgoing_can_data.length; i++) {
                        outgoing_can_data.data[i] = incomming_serial_data[4 + i];
                    }

                    if (!can_send_message(&outgoing_can_data)) led(false, true);
                    else switch_led(true, true);

                }
                else {  /// LED_REMOTE
                    PORTA |= incomming_serial_data[4];
                    PORTA &= ~(incomming_serial_data[5])
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
            else {
                led(false, true);
                _delay_ms(50);
                led(false, false);
                _delay_ms(50);
            }
        }
        if (PINE & 0xF0) {  /// button pressed
            if (!f_pressed) {
                // send the signal that a button was pressed ...
                char outgoing_serial_data[11];

                outgoing_serial_data[0] = 0;
                outgoing_serial_data[1] = BUTTON_PRESSED_ID;
                outgoing_serial_data[2] = 1;

                outgoing_serial_data[3] = PINE;

                for (int i = 0; i < 7; i++) {
                    outgoing_serial_data[4+i] = 0;
                }

                for (int i = 0; i < 11; i++) {
                    uart_putc(outgoing_serial_data[i]);
                }
            }
            f_pressed = true;
        }
        else f_pressed = false;
    }
}
