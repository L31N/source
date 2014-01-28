
#include "pwm.h"
#include <stdlib.h>
int * compare;

unsigned char offtime[3];
unsigned char ontime[3];
unsigned char timesum[3] = { 1 };   // ontime + offtime

unsigned char overflow_count[3] = { 0 };


void pwm_init()
{
    offtime[0] = 0;
    offtime[1] = 0;
    offtime[2] = 0;
    ontime[0] = 1;
    ontime[1] = 1;
    ontime[2] = 1;


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

void pwm_set_ontime(unsigned char _ontime, unsigned char num) {
    ontime[num] = _ontime;
    timesum[num] = ontime[num] + offtime[num];
}

void pwm_set_offtime(unsigned char _offtime, unsigned char num) {
    offtime[num] = _offtime;
    timesum[num] = ontime[num] + offtime[num];
}


ISR(TIMER1_OVF_vect )
{
	TCNT1 = 45536;  // 10ms interval

    PORTB &= ~((1<<4)|(1<<5)|(1<<6));

	overflow_count[0] ++;
	overflow_count[1] ++;
	overflow_count[2] ++;

    // set to zero if periode is over
	/*overflow_count[0] %= ontime[0] + offtime[0];
	overflow_count[1] %= ontime[1] + offtime[1];
	overflow_count[2] %= ontime[2] + offtime[2];*/
	// modulo is slower than ifs ...

	if (overflow_count[0] >= timesum[0]) overflow_count[0] = 0;
	if (overflow_count[1] >= timesum[1]) overflow_count[1] = 0;
	if (overflow_count[2] >= timesum[2]) overflow_count[2] = 0;
}

ISR(TIMER1_COMPA_vect )
{
    if (overflow_count[0] < ontime[0]) {
        OCR1A = compare[0]; // set on
    }
    else {
        OCR1A = 62535;  // set off
    }
    PORTB |= (1<<4);
}

ISR(TIMER1_COMPB_vect )
{
    if (overflow_count[1] < ontime[1]) {
        OCR1B = compare[1]; // set on
    }
    else {
        OCR1B = 62535;  // set off
    }
	PORTB |= (1<<5);
}

ISR(TIMER1_COMPC_vect )
{
    if (overflow_count[2] < ontime[2]) {
        OCR1C = compare[2]; // set on
    }
    else {
        OCR1C = 62535;  // set off
    }
	PORTB |= (1<<6);
}
