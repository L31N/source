/*
 */

#include <avr/io.h>
#include <util/delay.h>

#include "uart.h"


bool spi_init(void);
bool spi_write(unsigned char address, unsigned char data);
bool spi_read(unsigned char address, unsigned char& data);

unsigned short get_temperature(void);


int main(void) {
    //uart_init(38400);
    spi_init();

    _delay_ms(50);

    unsigned char tmp;
    //spi_read(0x42, tmp);
    spi_read(0x42, tmp);
    while(true);
    return 0;

    //spi_write(0x42, 0x42);

    //while(true);

    /*while(true) {
        PORTB ^= (1 << 2);
        _delay_ms(100);
    }*/

    while(true) {
        //uart_putc(0xFE);



        //unsigned short temperature = get_temperature();

        //uart_putc((unsigned char)(temperature >> 8));
        //uart_putc((unsigned char)temperature);

        _delay_ms(500);
    }

}

bool spi_init() {

    // Set CS, MOSI and SCK output, all others input
    DDRB |= (1<<3);
    DDRB |= (1<<2);
    DDRB |= (1<<5);

    // Set SPI-Enable, SPI-Master, 1MHz
    SPCR |= ( (1 << SPE) | (1 << MSTR) | (1 << SPR0) );

    // SS high
    PORTB |= (1 << 2);

    return true;
}

bool spi_write(unsigned char address, unsigned char data) {
    if (address & 0x80) return false;

    // SS low
    PORTB &= ~(1 << 2);

    // write address to SPI-Data-Register
    SPDR = address;
    // wait till transmission is complete
    while(!(SPSR & (1<<SPIF)));

    SPDR = data;
    while(!(SPSR & (1<<SPIF)));

    // SS high
    PORTB |= (1 << 2);

    return true;
}

bool spi_read(unsigned char address, unsigned char& data) {
    if (address & 0x80) return false;

    // SS low
    PORTB &= ~(1 << 2);

    // write address to SPI-Data-Register
    SPDR = address | 0x80;

    // wait for transmission completed
    while(!(SPSR & (1<<SPIF)));

    //write dummy byte
    SPDR = 0x00;
    while(!(SPSR & (1<<SPIF)));

    // write received data to buffer
    data = SPDR;

    // SS high
    PORTB |= (1 << 2);

    return true;
}

unsigned short get_temperature(void) {
    unsigned char templ, temph;

    spi_read(0x41, temph);
    spi_read(0x42, templ);

    unsigned short temp = 0;
    temp |= templ;
    temp |= (temph << 8);

    return temp;
}
