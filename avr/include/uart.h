#ifndef UART_H_INCLUDED
#define UART_H_INCLUDED

#include <avr/io.h>
#include <avr/interrupt.h>

//Prozessorpins definieren
//ATmega8
#if defined (__AVR_ATmega8__)
	#define UART_BUFFER_SIZE 100

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
	#define UART_BUFFER_SIZE 200

	#define TWO_UARTS

    #define UART_TX_VECTOR USART0_UDRE_vect
    #define UART_RX_VECTOR USART0_RX_vect
    #define UART_UDR UDR0
    #define UART_STATUS_B UCSR0B
    #define UART_BAUD_HIGH UBRR0H
    #define UART_BAUD_LOW UBRR0L
    #define UART_STATUS_C() (UCSR0C = (1 << UCSZ01) | (1 << UCSZ00))

    #define UART1_TX_VECTOR USART1_UDRE_vect
    #define UART1_RX_VECTOR USART1_RX_vect
    #define UART1_UDR UDR1
    #define UART1_STATUS_B UCSR1B
    #define UART1_BAUD_HIGH UBRR1H
    #define UART1_BAUD_LOW UBRR1L
    #define UART1_STATUS_C() (UCSR1C = (1 << UCSZ11) | (1 << UCSZ10))
#endif

//AT90CAN128
#if defined (__AVR_AT90CAN128__)
	#define UART_BUFFER_SIZE 500

	#define TWO_UARTS

    #define UART_TX_VECTOR USART0_UDRE_vect
    #define UART_RX_VECTOR USART0_RX_vect
    #define UART_UDR UDR0
    #define UART_STATUS_B UCSR0B
    #define UART_BAUD_HIGH UBRR0H
    #define UART_BAUD_LOW UBRR0L
    #define UART_STATUS_C()

    #define UART1_TX_VECTOR USART1_UDRE_vect
    #define UART1_RX_VECTOR USART1_RX_vect
    #define UART1_UDR UDR1
    #define UART1_STATUS_B UCSR1B
    #define UART1_BAUD_HIGH UBRR1H
    #define UART1_BAUD_LOW UBRR1L
    #define UART1_STATUS_C()
#endif


extern volatile unsigned char uart_tx_buffer[UART_BUFFER_SIZE];
extern volatile short uart_tx_write;
extern volatile short uart_tx_read;

extern volatile unsigned char uart_rx_buffer[UART_BUFFER_SIZE];
extern volatile short uart_rx_write;
extern volatile short uart_rx_read;

ISR(USART_UDRE_vect);
ISR(USART_RXC_vect);

void uart_init(long baud);
unsigned char uart_putc(unsigned char c);
unsigned char uart_getc();
unsigned char uart_putstr(unsigned char str[]);
unsigned char uart_write(unsigned char* buffer, unsigned short len);
unsigned char uart_isnewdata();
unsigned char uart_read(unsigned char *str, int count);
unsigned int uart_count();
void uart_clear();

//Definitions für the second uart on some devices
#if defined (TWO_UARTS)

extern volatile unsigned char uart1_tx_buffer[UART_BUFFER_SIZE];
extern volatile short uart1_tx_write;
extern volatile short uart1_tx_read;

extern volatile unsigned char uart1_rx_buffer[UART_BUFFER_SIZE];
extern volatile short uart1_rx_write;
extern volatile short uart1_rx_read;

ISR(USART1_UDRE_vect);
ISR(USART1_RXC_vect);

void uart1_init(long baud);
unsigned char uart1_putc(unsigned char c);
unsigned char uart1_getc();
unsigned char uart1_putstr(unsigned char str[]);
unsigned char uart1_isnewdata();
unsigned char uart1_read(unsigned char *str, int count);
unsigned int uart1_count();
void uart1_clear();

#endif

#endif // UART_H_INCLUDED
