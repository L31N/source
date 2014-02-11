#ifndef CLOCK_H_INCLUDED
#define CLOCK_H_INCLUDED

#include <avr/io.h>
#include <avr/interrupt.h>

void clock_init();

unsigned long long clock_get();

unsigned long clock_getoverflow();


#endif // CLOCK_H_INCLUDED
