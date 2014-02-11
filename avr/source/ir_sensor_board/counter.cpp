
#include "counter.h"

unsigned int counter[8];
unsigned int counterCurrent[8];

//const unsigned int preload_value = (unsigned int)65535UL-(4*15625UL);    // 65535-ms*15625 -- not more than 4 (no negative values)
const unsigned int preload_value =  65535 - 5*250;   // 65535-ms*15625 -- not more than 4 (no negative values)

void counter_init()
{
    sei();

    //Reset timer
    TCNT1 = preload_value;
    //Enable overflow interrupt
    TIMSK1 |= (1 << 0);
    //Enable timer with prescalor 1
    TCCR1B |= (1 << 0) | (1 << 1);

    //Set on rising edge
    EICRA = 0xFF;
    EICRB = 0xFF;

    //Enable interrupts
    EIMSK = 0xFF;
}

unsigned int counter_get(unsigned char chan)
{
    if(chan > 7) return 0;
    else return counter[chan];
}

ISR(TIMER1_OVF_vect)
{
    TCNT1 = preload_value;

    counter[0] = counterCurrent[0];
    counter[1] = counterCurrent[1];
    counter[2] = counterCurrent[2];
    counter[3] = counterCurrent[3];
    counter[4] = counterCurrent[4];
    counter[5] = counterCurrent[5];
    counter[6] = counterCurrent[6];
    counter[7] = counterCurrent[7];

    counterCurrent[0] = 0;
    counterCurrent[1] = 0;
    counterCurrent[2] = 0;
    counterCurrent[3] = 0;
    counterCurrent[4] = 0;
    counterCurrent[5] = 0;
    counterCurrent[6] = 0;
    counterCurrent[7] = 0;
}

ISR(INT0_vect)
{
    counterCurrent[0]++;
}

ISR(INT1_vect)
{
    counterCurrent[1]++;
}

ISR(INT2_vect)
{
    counterCurrent[2]++;
}
ISR(INT3_vect)
{
    counterCurrent[3]++;
}

ISR(INT4_vect)
{
    counterCurrent[4]++;
}

ISR(INT5_vect)
{
    counterCurrent[5]++;
}

ISR(INT6_vect)
{
    counterCurrent[6]++;
}
ISR(INT7_vect)
{
    counterCurrent[7]++;
}

