
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void sleep(long delay);
void pwm_init();
void pwm_set(unsigned int time);
int pwm_get();


void sleep(long delay)
{
	long i;

	for(i=0 ; i<delay ; i++)
	{
		_delay_ms(1);
	}
}

void pwm_init()
{
	//Counter auf Null setzten
	TCNT1 = 0;

	//Interrupts aktivieren
	TIMSK |= (1<<2)|(1<<4);
	sei();

	//Port einschalten
	DDRD |= (1<<7);

	//Timer init setzen
	pwm_set(1500);

	//Timer starten
	TCCR1B |= (1<<1);

}

void pwm_set(unsigned int time)
{
	OCR1A = 65535 - 2*time;
}

int pwm_get()
{
	if(PIND & (1<<6))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

ISR(TIMER1_OVF_vect )
{
	TCNT1 = 25536;

	PORTD &= ~(1<<7);
}

ISR(TIMER1_COMPA_vect )
{
	PORTD |= (1<<7);
}

int main()
{
	DDRB |= (1<<0);
	PORTB |= (1<<0);

	pwm_init();

	pwm_set(2000);

	//Warten auf Reglerstart
	while(!pwm_get());

	PORTB &= ~(1<<0);

	//Sequenz abwarten
	sleep(2000);
	sleep(10000);
	sleep(3000);
	sleep(5000);

	//Gewechselt in Programmiermodus

	pwm_set(1500);
	sleep(1000);
	pwm_set(2000);
	sleep(3000);

	pwm_set(1500);
	sleep(1000);
	pwm_set(2000);
	sleep(3000);

	//Option auswählen
	pwm_set(1500);
	sleep(10000);

	//rückwärts on
	pwm_set(2000);
	sleep(1000);
	pwm_set(1500);
	sleep(1000);

	sleep(5000);

	pwm_set(2000);

	sleep(5000);


	//Wechseln auf Position 5
	pwm_set(1500);
	sleep(1000);
	pwm_set(2000);
	sleep(3000);

	pwm_set(1500);
	sleep(1000);
	pwm_set(2000);
	sleep(3000);

	//Option auswählen
	pwm_set(1500);
	sleep(10000);

	//Einstellung auswählen, noch unbekannt

	pwm_set(2000);
	sleep(1000);
	pwm_set(1500);
	sleep(1000);

	sleep(5000);

	pwm_set(2000);

	sleep(5000);

	//Restart warten
	PORTB |= (1<<0);
	while(pwm_get());
	pwm_set(2000);
	sleep(500);
	while(!pwm_get());
	PORTB &= ~(1<<0);

	sleep(2000);
	sleep(10000);
	sleep(500);

	pwm_set(1500);
	sleep(2000);
	pwm_set(1000);
	sleep(2000);

	PORTB |= (1<<0);




	//Bestätigen
//	pwm_set(2000);
//	sleep(10000);

    while(1);

    return 0;
}
