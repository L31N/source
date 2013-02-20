

#include "adc.h"

void adc_init()
{
	//ADC aktivieren
	ADCSRA |= (1 << 7);

	//Takt setzen
	ADCSRA |= 0x07;

	//Externe Referenz setzen
	ADMUX |= (1<<6);

	//Kanal auf 0 setzen
	ADMUX &= ~(0x1F);
}

unsigned short adc_read(unsigned char pin)
{
	//Falscher Pin -> Abbruch
	if(pin > 7) return 0;

	//Pin setzen
	ADMUX &= ~(0x1F);
	ADMUX |= pin;

	 //Wandlung starten
	 ADCSRA |= (1<<6);

	 //Warten bis Wandlung abgeschlossen
	 while(ADCSRA & (1<<6));

	 //Wert auslesen
	 short tmp = 0;
	 tmp |= ADCL;
	 tmp |= (ADCH << 8);

	 //Wert zurückgeben
	 return tmp;
}
