
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include <stdint.h>

#include "can.h"
#include "can_testing.h"


prog_uint8_t can_filter[] = {
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
};

int main () {
    init_leds();

    led(true, true);
    _delay_ms(500);
    led(false, false);
    _delay_ms(500);

    /** CAN TESTING HERE **/
    can_init(BITRATE_500_KBPS);

    //can_static_filter(can_filter);

    led(0, 1);
    _delay_ms(500);
    led(0, 0);

    /// DEFINING FILTERS
    // filter 0
    can_filter_t filter0;
    filter0.id = 0x02345670;
    filter0.mask = 0x1FFFFFF0;
    filter0.flags.rtr = 0;
    filter0.flags.extended = 0;

    // filter 1
    can_filter_t filter1;
    filter1.id = 0x02345660;
    filter1.mask = 0x1FFFFFF0;
    filter1.flags.rtr = 0;
    filter1.flags.extended = 0;

    // other filters
    can_filter_t filterX = filter1;

    can_set_filter(0, &filter0);
    can_set_filter(1, &filter1);
    can_set_filter(2, &filterX);
    can_set_filter(3, &filterX);
    can_set_filter(4, &filterX);
    can_set_filter(5, &filterX);

    while(true) {
        if (can_check_message()) {
            can_t msg;
            if (can_get_message(&msg)) {
                led(true, true);
                _delay_ms(10);
                led(false, false);
            }
            else {
                led(false, true);
                _delay_ms(10);
                led(false, false);
            }
        }
    }



    /** ** ++++++++++ ** **/

    return 0;
}
