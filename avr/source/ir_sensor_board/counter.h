#ifndef CLOCK_H_INCLUDED
#define CLOCK_H_INCLUDED

#include <avr/io.h>
#include <avr/interrupt.h>

void counter_init();

unsigned int counter_get(unsigned char chan);

#endif // CLOCK_H_INCLUDED
