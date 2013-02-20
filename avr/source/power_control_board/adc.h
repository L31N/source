#ifndef ADC_H_INCLUDED
#define ADC_H_INCLUDED

#include <avr/io.h>

void adc_init();

unsigned short adc_read(unsigned char pin);



#endif // ADC_H_INCLUDED
