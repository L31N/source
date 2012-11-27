
#include <avr/io.h>
#include <util/delay.h>

#include "uart.h"

int main(void)
{
    //DDRB |= (1 << 0);
    //DDRB |= (1 << 1);

    //PORTB = 0x03;
    //_delay_ms(500);
    //PORTB = 0x00;
    //_delay_ms(500);

    uart_init(57600);

    uart_putstr("\rhello_world_i_am\n");
    //for (int i = 0; i < 10; i++) uart_putc('a');

    _delay_ms(500);

    //PORTB = 0x03;
    //_delay_ms(500);
    //PORTB = 0x00;


    /*unsigned char c;

    while(1)
    {
        c = uart_getc();

        if(c == 'e')
        {
            PORTB |= (1 << 0);
        }
        else if(c == 'a')
        {
            PORTB &= ~(1 << 0);
        }
    }*/

    return 0;
}
