

#include "uart.h"

volatile char uart_tx_buffer[UART_BUFFER_SIZE];
volatile short uart_tx_write;
volatile short uart_tx_read;

volatile char uart_rx_buffer[UART_BUFFER_SIZE];
volatile short uart_rx_write;
volatile short uart_rx_read;


ISR(UART_TX_VECTOR)
{
    if(uart_tx_read == uart_tx_write)
    {
        UART_STATUS_B &= ~(1 << 5);
        return;
    }

    UART_UDR = uart_tx_buffer[uart_tx_read];

    uart_tx_read++;

    if(uart_tx_read >= UART_BUFFER_SIZE)
    {
        uart_tx_read = 0;
    }
}


ISR(UART_RX_VECTOR)
{
    if(uart_rx_write + 1 == uart_rx_read || (uart_rx_read == 0 && uart_rx_write + 1 == UART_BUFFER_SIZE))
    {
        UART_UDR;
        return;
    }

    uart_rx_buffer[uart_rx_write] = UART_UDR;
	uart_rx_write++;

    if(uart_rx_write >= UART_BUFFER_SIZE)
    {
        uart_rx_write = 0;
    }
}


void uart_init(long baud)
{
    //Senden aktivieren
    UART_STATUS_B |= (1 << 3);

    //Empfang aktivieren
    UART_STATUS_B |= (1 << 4);

    //Baud setzen
    short UBRR = (F_CPU/(16*baud))-1;
    UART_BAUD_HIGH = UBRR >> 8;
    UART_BAUD_LOW = UBRR & 0xFF;
    UART_STATUS_C();

    //Interrupts global aktivieren
    sei();

    //Empfangsinterrupt aktivieren
    UART_STATUS_B |= (1 << 7);

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

	UART_STATUS_B |= 1 << 5;

    return 0;
}

unsigned char uart_getc()
{
    if(uart_rx_read == uart_rx_write)
    {
        return 0;
    }

    unsigned char tmp = uart_rx_buffer[uart_rx_read];

    uart_rx_read++;

    if(uart_rx_read >= UART_BUFFER_SIZE)
    {
        uart_rx_read = 0;
    }

    return tmp;
}

unsigned char uart_putstr(char str[])
{
    unsigned short i;

    for( i=0 ; str[i] != '\0' ; i++ )
    {
        if( uart_putc(str[i]) != 0 )
        {
            return 1;
        }
    }

    return 0;
}


unsigned char uart_isnewdata()
{
    if(uart_rx_read == uart_rx_write)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

