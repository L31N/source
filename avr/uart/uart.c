

#include "uart.h"

ISR(USART_UDRE_vect)
{
    if(uart_tx_read == uart_tx_write)
    {
        PORTB |= (1 << 0);
        UCSRB &= ~(1 << 5);
        return;
    }

    UDR = uart_tx_buffer[uart_tx_read];

    uart_tx_read++;

    if(uart_tx_read >= UART_BUFFER_SIZE)
    {
        uart_tx_read = 0;
    }
}

/*
ISR(USART_RXC_vect)
{
    *uart_rx_write = UDR;
    uart_rx_write++;

    //Pufferüberlauf
    if(uart_rx_read == &uart_rx_buffer[UART_BUFFER_SIZE-1])
    {
        uart_rx_read = uart_tx_buffer;
    }
}*/

void uart_init(long baud)
{
    //Senden aktivieren
    UCSRB |= (1 << 3);

    //Empfang aktivieren
    //UCSRB |= (1 << 4);

    //Baud setzen
    short UBRR = (F_CPU/(16*baud))-1;
    UBRRH = UBRR >> 8;
    UBRRL = UBRR & 0xFF;
    UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);

    //Interrupts global aktivieren
    sei();

    //Empfangsinterrupt aktivieren
    //UCSRB |= (1 << 5);

    //Bufferzeiger setzen
    uart_tx_read = 0;
    uart_tx_write = 0;

    uart_rx_read = 0;
    uart_rx_write = 0;

}


unsigned char uart_putc(char c)
{
    if(uart_tx_write + 1 == uart_tx_read || (uart_tx_read == 0 && uart_tx_write + 1 == UART_BUFFER_SIZE))
    {
        return 1;
    }

    uart_tx_buffer[uart_tx_write] = c;
	uart_tx_write++;

    if(uart_tx_write >= UART_BUFFER_SIZE)
    {
        uart_tx_write = 0;
    }
	
	UCSRB |= 1 << 5;

    return 0;
}
