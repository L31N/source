
#include <avr/io.h>

#include "uart.h"


int main ()
{
    bool flag = false;

    uart_init(38400);
    while(1)
    {
        if(uart_isnewdata())
        {
            if(flag)
            {
                uart_putc (uart_getc());
            }
            else
            {
                uart_putc (uart_getc());
            }

            flag = !flag;

        }


    }
}
