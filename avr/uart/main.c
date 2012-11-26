
#include <avr/io.h>
#include <util/delay.h>

#include "uart.h"

int main(void)
{
    DDRB |= (1 << 0);

    uart_init(57600);

    uart_putstr("\r\nTestprogramm für den UART von Soctronic.de\r\n");

    unsigned char c;

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
    }

    return 0;
}
