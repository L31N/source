
	#include <avr/io.h>
	#include <avr/interrupt.h>
	#include <util/delay.h>

	#include "can.h"
	#include "adc.h"

int main(void)
{
	adc_init();

	can_init(BITRATE_100_KBPS);
	sei();

    can_filter_t filter0;
    filter0.id = 65;        /// board0
    //filter0.id = 97;        /// board1
    filter0.mask = 0x7E0;

    filter0.flags.rtr = 0;

    can_set_filter(0, &filter0);

	while(1)
	{
		can_t data_laser_0;
		can_t data_laser_1;
		can_t data_reflex;

		data_laser_0.flags.rtr = 0;
		data_laser_0.id = 65;
		data_laser_0.length = 8;

		data_laser_1.flags.rtr = 0;
		data_laser_1.id = 66;
		data_laser_1.length = 2;

		data_reflex.flags.rtr = 0;
		data_reflex.id = 67;
		data_reflex.length = 1;

		unsigned short adc_laser_0;
		unsigned short adc_laser_1;
		bool status_reflex;

		adc_laser_0 = adc_read(0);
		//adc_laser_0 = 2506;
		adc_laser_1 = adc_read(1);
		status_reflex = ( (unsigned long)PINE & (1<<4) ) >> 4;

        //data_laser_0.data[0] = adc_laser_0 & 0xFF00;
        //data_laser_0.data[1] = adc_laser_0 & 0x00FF;
        data_laser_0.data[0] = adc_laser_0;
        data_laser_0.data[1] = adc_laser_0 >> 8;
        for (int i = 2; i < 8; i++) data_laser_0.data[i] = 255;

        data_laser_1.data[0] = adc_laser_1 & 0xFF00;
        data_laser_1.data[1] = adc_laser_1 & 0x00FF;
        for (int i = 2; i < 8; i++) data_laser_1.data[i] = 255;

        data_reflex.data[0] = status_reflex;
        for (int i = 1; i < 8; i++) data_reflex.data[i] = 255;

        /*data_laser_0.data[0] = 243;
        data_laser_0.data[1] = 'A';
        data_laser_0.data[2] = 'B';
        data_laser_0.data[3] = 'C';
        data_laser_0.data[4] = 'D';
        data_laser_0.data[5] = 'E';
        data_laser_0.data[6] = 'F';
        data_laser_0.data[7] = 'G';

        data_laser_1.data[0] = 0x20;
        data_laser_1.data[1] = 0x00;

        data_reflex.data[0] = 1;*/


        can_send_message (&data_laser_0);
        can_send_message (&data_laser_1);
        can_send_message (&data_reflex);

        _delay_ms(500);
	}


    return 0;
}
