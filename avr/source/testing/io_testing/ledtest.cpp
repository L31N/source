
#include <avr/io.h>
#include <util/delay.h>

int main () {
    DDRB = 0x01;
    //PORTB |= 0x01;
    //sbi(PORTB, 0);

    while(true) {
        PORTB |= 0x01;
        _delay_ms(500);
        PORTB &= ~0x01;
        _delay_ms(500);
    }

    return 0;
}

