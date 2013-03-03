
#include "pwm.h"
#include <stdlib.h>
int * compare;

void pwm_init()
{
    compare = (int*)malloc(3*sizeof(int));
	//Counter auf Null setzten
	TCNT1 = 0;

	//Interrupts aktivieren
	TIMSK1 |= (1<<0)|(1<<1)|(1<<2)|(1<<3);
	sei();

	//Port einschalten
	DDRB |= (1<<4)|(1<<5)|(1<<6);

	//Timer init setzen
	pwm_set(500, 0);
	pwm_set(500, 1);
	pwm_set(500, 2);
	OCR1A = compare[0];
	OCR1B = compare[1];
	OCR1C = compare[2];

	//Timer starten
	TCCR1B |= (1<<1);





}

void pwm_set(unsigned int value, unsigned char num)
{
    if (value > 1000) return;       // error invalid value

	switch (num) {
        case 0:
            //OCR1A = 63535 - 2*value;
            compare[0] = 63535 - 2*value;
            break;
        case 1:
            //OCR1B = 63535 - 2*value;
            compare[1] = 63535 - 2*value;
            break;
        case 2:
            //OCR1C = 63535 - 2*value;
            compare[2] = 63535 - 2*value;
            break;
        default:
            break;
            // error wrong number
    }
}


ISR(TIMER1_OVF_vect )
{
	TCNT1 = 25536;

	PORTB &= ~((1<<4)|(1<<5)|(1<<6));
}

ISR(TIMER1_COMPA_vect )
{
    OCR1A = compare[0];
	PORTB |= (1<<4);
}

ISR(TIMER1_COMPB_vect )
{
    OCR1B = compare[1];
	PORTB |= (1<<5);
}

ISR(TIMER1_COMPC_vect )
{
    OCR1C = compare[2];
	PORTB |= (1<<6);
}
