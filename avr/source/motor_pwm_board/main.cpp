
#include <avr/io.h>
#include <util/delay.h>

#include "can.h"
#include "pwm.h"
#include "uart.h"

int main () {

    pwm_init();

    //const bool BOARD_0 = true;      /// board0
    const bool BOARD_0 = false;     /// board1

    unsigned short BOARD_ID = 1;

    //if (BOARD_0) BOARD_ID = 1;
    //else BOARD_ID = 33;

    uart_init(38400);

    can_init(BITRATE_100_KBPS);
    sei();

    // motor 1
    can_filter_t filter0;
    filter0.id = BOARD_ID;
    filter0.mask = 0x7E0;
    filter0.flags.rtr = 0;

    can_set_filter(0, &filter0);

    while(42) {
        if (can_check_message()) {
            can_t message;
            can_get_message(&message);

            if ((BOARD_0 && message.id > 0 && message.id < 3) || (!BOARD_0 && message.id > 2 && message.id < 5)) {
                // values +/- 255

                short tmp = 0;
                tmp = message.data[2];

                if(message.data[1] == 1)
                {
                    tmp *= -1;
                }

                if (!BOARD_0) tmp *= (-1);

                long value = tmp;

                value += 255;
                value *= 1000;
                value /= (2*255);

                //uart_debug("value: %d", value);

                if (message.data[0] == 0) pwm_set((unsigned long)value, 0);
                if (message.data[0] == 1) pwm_set((unsigned long)value, 1);

            }
        }
    }
}
