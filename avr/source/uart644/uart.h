
#ifndef UART_H_INCLUDED
#define UART_H_INCLUDED

#include <avr/io.h>
#include <avr/interrupt.h>

#define UART_BUFFER_SIZE 100

extern volatile char uart_tx_buffer[UART_BUFFER_SIZE];
extern volatile short uart_tx_write;
extern volatile short uart_tx_read;

extern volatile char uart_rx_buffer[UART_BUFFER_SIZE];
extern volatile short uart_rx_write;
extern volatile short uart_rx_read;

ISR(USART_UDRE_vect);
ISR(USART_RXC_vect);

void uart_init(long baud);

unsigned char uart_putc(char c);
unsigned char uart_getc();

unsigned char uart_putstr(const char str[]);


#endif // UART_H_INCLUDED