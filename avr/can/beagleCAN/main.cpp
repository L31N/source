
#include "uart.h"
#include "can.h"
#include "can_testing.h"

#include <stdlib.h>
#include <string.h>

#include <util/delay.h>

int main () {
    uart_init(115200);
    init_leds();
    can_init(BITRATE_1_MBPS);

    indicate_init();                // flashing LEDs

    can_filter_t filter0;
    filter0.id = 0x0;
    filter0.mask = 0x0;
    filter0.flags.rtr = 0;

    can_set_filter(0, &filter0);

    /*can_t {
        uint16_t id;				//!< ID der Nachricht (11 Bit)
		struct {
			int rtr : 1;			//!< Remote-Transmit-Request-Frame?
		} flags;

        uint8_t length;				//!< Anzahl der Datenbytes
        uint8_t data[8];			//!< Die Daten der CAN Nachricht
    }*/

    can_t outgoing_can_data;

    char* incomming_serial_data = (char*) malloc(13);
    char* outgoing_serial_data = (char*) malloc(12);

    while(true) {
        if (uart_isnewdata()) {      /// neue Uart messages vorhanden
            for (int i = 0; uart_isnewdata(); i++) {
                incomming_serial_data[i] = uart_getc();
                if (incomming_serial_data[i] == '\0') {
                    break;
                }
            }

            // analyze uart-message
            if(incomming_serial_data[0] == 's') {         /// send the message via CAN
                outgoing_can_data.flags.rtr = incomming_serial_data[1];
                outgoing_can_data.id = incomming_serial_data[2];
                outgoing_can_data.length = incomming_serial_data[3];
                for (int i = 0; i < outgoing_can_data.length; i++) {
                    outgoing_can_data.data[i] = incomming_serial_data[4 + i];
                }

                if (can_send_message(&outgoing_can_data)) {
                    led(true, false);
                    _delay_ms(1000);
                    led(false, false);
                }
                else {
                    led(false, true);
                    _delay_ms(1000);
                    led(false, false);
                }
            }
            else if (incomming_serial_data[0] == 'c') {  ///  set callibarations like filters etc.

            }
        }
        if (can_check_message()) {  /// neue CAN messages vorhanden
            can_t incomming_can_data;
            if (can_get_message(&incomming_can_data)) {

                memset(outgoing_serial_data, 0, 13);
                outgoing_serial_data[0] = incomming_can_data.flags.rtr;
                outgoing_serial_data[1] = incomming_can_data.id;
                outgoing_serial_data[2] = incomming_can_data.length;

                for (int i = 0; i < incomming_can_data.length; i++) {
                    outgoing_serial_data[3 + i] = incomming_can_data.data[i];
                }
                outgoing_serial_data[3+incomming_can_data.length] = '\0';

                //uart_putstr(outgoing_serial_data);

                for (int i = 0; i < 3+incomming_can_data.length; i++) {
                    uart_putc(*(outgoing_serial_data+i));
                }

                led(true, true);
                _delay_ms(50);
                led(false, false);
                _delay_ms(50);

                /*uart_putc(incomming_can_data.flags.rtr);
                uart_putc(incomming_can_data.id);
                uart_putc(incomming_can_data.length);

                for (int i = 0; i < incomming_can_data.length; i++) uart_putc(incomming_can_data.data[i]);*/

                //uart_putc('X');

            }
            else {
                led(false, true);
                _delay_ms(50);
                led(false, false);
                _delay_ms(50);
            }
        }
    }

    /*while(true) {
        uart_putc((char)outgoing_data.id);
        //uart_putc(' ');
        uart_putc((char)outgoing_data.flags.rtr);
        //uart_putc(' ');
        uart_putc((char)outgoing_data.length);
        //uart_putc(' ');

        for (int i = 0; i < outgoing_data.length; i++) {
            uart_putc(outgoing_data.data[i]);
            //uart_putc(' ');
        }

        led(true, true);
        _delay_ms(500);
        led(false, false);
        _delay_ms(500);
    }*/

    return 0;
}
