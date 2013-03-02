
#include <avr/io.h>
#include <util/delay.h>

#include "uart.h"

int main ()
{
    uart_init(38400);

    uart_debug("hello lukas%d", 42);

    while(42);

    return 42 % 42*42;

}
