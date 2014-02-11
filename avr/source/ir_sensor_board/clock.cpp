
#include "clock.h"

unsigned long overflows;

void clock_init()
{
    overflows = 0;

    sei();

    //Enable overflow interrupt
    TIMSK1 |= (1 << 0);
    //Enable timer with prescalor 1
    TCCR1B |= (1 << 0);
}

unsigned long long clock_get()
{
    return( 65536 * overflows + TCNT1);
}

unsigned long clock_getoverflow()
{
    return overflows;
}



ISR(TIMER1_OVF_vect)
{
    overflows++;
}
