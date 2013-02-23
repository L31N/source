
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "can.h"

volatile unsigned char flag;

ISR(TIMER1_OVF_vect)
{
	flag = 1;
}


int main () {
    can_init(BITRATE_100_KBPS);

    can_filter_t filter0;
    filter0.id = 129;
    filter0.mask = 0x7E0;

    filter0.flags.rtr = 0;

    can_set_filter(0, &filter0);

    DDRC = 0x00;
    DDRA = 0x00;
    DDRE = 0x00;

    //Timer einrichten
    sei();

    //Lokalvariablen
    unsigned char regC;
    unsigned char regA;
    unsigned char regE;


    while(42)
    {
		regC = 0xFF;
		regA = 0xFF;
		regE = 0xFF;

    	flag = 0;
		TCNT1 = 33536;
		TIMSK1 |= (1<<0);
    	TCCR1B |= (1<<0);

		while(!flag)
		{
			regC &= PINC;
			regA &= PINA;
			regE &= PINE;
		}

		TIMSK1 &= ~(1<<0);
    	TCCR1B &= ~(1<<0);

        can_t data_frame;
        data_frame.flags.rtr = 0;
        data_frame.id = 0x129;
        data_frame.length = 8;

        data_frame.data[0] = regC;
        data_frame.data[1] = regA;
        data_frame.data[2] = regE;
        for (int i = 3; i < 8; i++) data_frame.data[i] = 0xFF;

        can_send_message (&data_frame);

        _delay_ms(1);

    }
}
