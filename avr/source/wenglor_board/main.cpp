
	#include <avr/io.h>
	#include <avr/interrupt.h>
	#include <util/delay.h>

	#include "can.h"
	#include "adc.h"

int main(void)
{
	adc_init();

	can_init(BITRATE_100_KBPS);

    can_filter_t filter0;
    filter0.id = 65;        /// board0
    filter0.id = 97;        /// board1
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
		data_laser_0.length = 2;

		data_laser_0.flags.rtr = 0;
		data_laser_0.id = 66;
		data_laser_0.length = 2;

		data_reflex.flags.rtr = 0;
		data_reflex.id = 67;
		data_reflex.length = 1;

		unsigned short adc_laser_0;
		unsigned short adc_laser_1;
		bool status_reflex;

		adc_laser_0 = adc_read(0);
		adc_laser_1 = adc_read(1);
		status_reflex = ( (unsigned long)PINE & (1<<4) ) >> 4;

        data_laser_0.data[0] = adc_laser_0 & 0x00FF;
        data_laser_0.data[1] = adc_laser_0 & 0xFF00;

        data_laser_1.data[0] = adc_laser_1 & 0x00FF;
        data_laser_1.data[1] = adc_laser_1 & 0xFF00;

        data_reflex.data[0] = status_reflex;


        can_send_message (&data_laser_0);
        can_send_message (&data_laser_1);
        can_send_message (&data_reflex);

        _delay_ms(2);
	}


    return 0;
}
