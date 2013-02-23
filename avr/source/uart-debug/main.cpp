
#include <avr/io.h>
#include <util/delay.h>

#include "uart.h"

int main ()
{
    uart_init(57600);

    while(true) {
        if (uart_count() >= 12) {
            char buffer[12];
            uart_read(buffer, 12);
            for (int i = 0; i < 12; i++) uart_putc(buffer[i]);
        }
    }

}
