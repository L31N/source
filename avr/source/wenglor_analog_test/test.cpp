#include <avr/io.h>
#include <util/delay.h>

#include "uart.h"
#include "adc.h"

int main()
{
	uart_init(38400);
	adc_init();

	while(1)
	{
		unsigned short value = adc_read(0);
		float distance = value * 3.5934;
		distance += 59.1305;

		uart_debug("%d\n\r", int(distance));

		/*unsigned char bytedis[2];
		bytedis[0] = (unsigned char)(distance>>8);
		bytedis[1] = (unsigned char)(distance);
		uart_putc(bytedis[0]);
		uart_putc(bytedis[1]);
		uart_putc(0xFF);
		uart_putc(0xFF);*/
		_delay_ms(100);
	}
	return 0;
}
