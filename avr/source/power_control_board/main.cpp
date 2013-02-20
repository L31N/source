
	#include <avr/io.h>
	#include <avr/interrupt.h>
	#include <util/delay.h>

	#include "can.h"
	#include "adc.h"

int main(void)
{
	adc_init();

	can_init(BITRATE_1_MBPS);

    can_filter_t filter0;
    filter0.id = 193;
    filter0.mask = 0x7FF;

    filter0.flags.rtr = 0;

    can_set_filter(0, &filter0);

	while(1)
	{
		can_t data_frame;
        data_frame.flags.rtr = 0;
        data_frame.id = 0x000;
        data_frame.length = 8;

        data_frame.data[0] = adc_read(0);
        data_frame.data[1] = adc_read(1);
        data_frame.data[2] = adc_read(2);
        data_frame.data[3] = adc_read(3);
        data_frame.data[4] = adc_read(4);
        data_frame.data[5] = adc_read(5);
        data_frame.data[6] = adc_read(6);
        data_frame.data[7] = adc_read(7);


        can_send_message (&data_frame);

        _sleep_ms(2000);
	}


    return 0;
}
