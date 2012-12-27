#ifndef UART_H_INCLUDED
#define UART_H_INCLUDED

#include <avr/io.h>
#include <avr/interrupt.h>

#define UART_BUFFER_SIZE 100

//Prozessorpins definieren
//ATmega8
#if defined (__AVR_ATmega8__)
    #define UART_TX_VECTOR USART_UDRE_vect
    #define UART_RX_VECTOR USART_RXC_vect
    #define UART_UDR UDR
    #define UART_STATUS_B UCSRB
    #define UART_BAUD_HIGH UBRRH
    #define UART_BAUD_LOW UBRRL
    #define UART_STATUS_C() (UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0))
#endif

//ATmega644
#if defined (__AVR_ATmega644__)
    #define UART_TX_VECTOR USART0_UDRE_vect
    #define UART_RX_VECTOR USART0_RX_vect
    #define UART_UDR UDR0
    #define UART_STATUS_B UCSR0B
    #define UART_BAUD_HIGH UBRR0H
    #define UART_BAUD_LOW UBRR0L
    #define UART_STATUS_C() (UCSR0C = (1 << UCSZ01) | (1 << UCSZ00))
#endif

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

unsigned char uart_putstr(char str[]);

unsigned char uart_isnewdata();

unsigned char uart_read(char *str, int count);

unsigned int uart_count();



#endif // UART_H_INCLUDED
