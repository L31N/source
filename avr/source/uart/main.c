
#include <avr/io.h>
#include <util/delay.h>

#include "uart.h"

int main(void)
{
    DDRB |= (1 << 0);
    PORTB |= (1 << 0);

    uart_init(57600);

    //uart_putstr("\r\nTestprogramm für den UART von Soctronic.de");

    _delay_ms(4000);

    uart_putstr("Ausgabe");

    _delay_ms(1000);

    int i;

    for(i=0; i<uart_count(); i++)
    {
        uart_putc('a');
    }

    char tmp[11];
    tmp[10]='\0';

    uart_read(tmp, 10);
    uart_putstr(tmp);

    while(1);
}
