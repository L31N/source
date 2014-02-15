
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <string.h>

#include "can.h"

#include "counter.h"


int main() {
    counter_init();

    can_init(BITRATE_1_MBPS);
    sei();

    can_filter_t filter0;
    filter0.id = 154;
    filter0.mask = 0x7E0;

    filter0.flags.rtr = 0;

    can_set_filter(0, &filter0);

    can_t frame;
    frame.flags.rtr = 0;
    frame.id = 153;
    frame.length = 8;

    while(true) {


        /*for (int i = 0; i < 8; i++) {       // expand to 8 sensors
            frame.data[i] = (unsigned char)counter_get(i);
        }*/

        can_t frameold = frame;

        frame.data[0] = (unsigned char)counter_get(0);  // set right order !
        frame.data[1] = (unsigned char)counter_get(1);
        frame.data[2] = (unsigned char)counter_get(2);
        frame.data[3] = (unsigned char)counter_get(3);

        frame.data[4] = (unsigned char)counter_get(4);
        frame.data[5] = (unsigned char)counter_get(7);
        frame.data[6] = (unsigned char)counter_get(6);
        frame.data[7] = (unsigned char)counter_get(5);


        if(memcmp(&frame, &frameold, sizeof(frame)) != 0) can_send_message(&frame);

        _delay_ms(30);
    }
}



/*volatile unsigned char flag;

ISR(TIMER1_OVF_vect)
{
	flag = 1;
}


int main () {
    can_init(BITRATE_100_KBPS);
    sei();

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
        data_frame.id = 129;
        data_frame.length = 8;

        data_frame.data[0] = ~regC;
        data_frame.data[1] = ~regA;
        data_frame.data[2] = ~regE;
        for (int i = 3; i < 8; i++) data_frame.data[i] = 0xFF;

        can_send_message (&data_frame);

        _delay_ms(1);

    }
}*/
