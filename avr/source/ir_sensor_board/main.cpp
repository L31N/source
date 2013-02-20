
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
    can_init(BITRATE_1_MBPS);

    can_filter_t filter0;
    filter0.id = 0x00F;
    filter0.mask = 0x7FF;

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
        data_frame.id = 0x000;
        data_frame.length = 3;

        data_frame.data[0] = regC;
        data_frame.data[1] = regA;
        data_frame.data[2] = regE;

        can_send_message (&data_frame);

    }
}
