
#ifndef _PWM_H_
#define _PWM_H_

#include <avr/io.h>
#include <avr/interrupt.h>

void pwm_init();
void pwm_set(unsigned int value, unsigned char num);

ISR(TIMER1_OVF_vect );
ISR(TIMER1_COMPA_vect );
extern int* comapre;

#endif // _PWM_H_
