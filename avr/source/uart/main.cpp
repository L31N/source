
#include <avr/io.h>
#include <util/delay.h>

#include "uart.h"

/*int main(void)
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
}*/

int main () {
    DDRB |= (1 << 0);
    PORTB |= (1 << 0);

    uart_init(57600);

    while(true) {
        if (uart_count() >= 12) {
            char buffer[12];
            uart_read(buffer, 12);
            for (int i = 0; i < 12; i++) uart_putc(buffer[i]);
        }
    }

}
