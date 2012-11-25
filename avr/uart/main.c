
#include <avr/io.h>
#include <util/delay.h>

#include "uart.h"

int main(void)
{
    DDRB |= (1 << 0);

    uart_init(9600);

    while(1)
    {
        uart_putc('a');
        _delay_ms(500);
    }

    /*unsigned char c;

    for(c = 0 ; c < 50 ; c++)
    {
        if(uart_putc('a') != 0)
        {
            //PORTB |= (1 << 0);
        }
        _delay_ms(1000);
    }

/*
    _delay_ms(500);

    for(c = 0 ; c < 80 ; c++)
    {
        uart_putc('a');
    }

    while(1);
    /*{
        PORTB |= (1 << 0);
        _delay_ms(500);
        PORTB &= ~(1 << 0);
        _delay_ms(500);
    }*/

    return 0;
}
