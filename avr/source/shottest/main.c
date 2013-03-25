/*
 */

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{

    // Insert code

    DDRA = 0x01;
    PORTA = 0x01;
    _delay_ms(1000);

    int i = 0;
    int counter = 0;
    while(42)
    {


        PORTA = 0x00;
        while( i > counter)
        {
            _delay_ms(1);
            counter++;
        }
        PORTA = 0x01;
        _delay_ms(2000);
        i += 10;
        counter = 0;
    }

    return 0;
}
