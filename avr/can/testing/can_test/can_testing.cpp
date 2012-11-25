
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "can.h"
#include "can_testing.h"

void init_leds() {
    DDRB = 0b00000011;  // set pins for LEDs as output
    return;
}

void led (bool green, bool red) {
    if (green) PORTB |= (1 << 0);
    else PORTB &= ~(1 << 0);

    if (red) PORTB |= (1 << 1);
    else PORTB &= ~(1 << 1);
}
