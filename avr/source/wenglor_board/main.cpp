
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/delay.h>

#include "can.h"
#include "uart.h"


const uint32_t ERROR_OVERFLOW_IN = 4294967293UL;
const uint32_t ERROR_OVERFLOW = 0xAAAAAAAD;
const uint32_t ERROR_NO_TARGET = 0xAAAAAAAE;
const uint32_t ERROR_INVAL_NUM = 0xAAAAAAAC;
const uint32_t ERROR_INVAL_CHECKSUM = 0xAAAAAAAB;
const unsigned char CANID = 0x00;

uint32_t getDistance (unsigned char sensor_num);       // needs called the uart_init() function !!!
bool getReflex();       /// not valid yet !!!

int main(void) {
    can_init(BITRATE_10_KBPS);
    sei();

    uart_init(38400);
    uart1_init(38400);

    can_t can_data;
    can_data.id = CANID;
    can_data.flags.rtr = false;
    can_data.length = 7;

    uint32_t distance[2] = { 0 };

    while(true) {

        // read the distance laser sensors
        distance[0] = getDistance(0);
        distance[1] = getDistance(1);

        for (int i = 0; i < 3; i++) can_data.data[i] = ( distance[0] >> i*8 ) & 0xFF;
        for (int i = 0; i < 3; i++) can_data.data[i+3] = ( distance[1] >> i*8 ) & 0xFF;

        // fill in the digital sensor value
        can_data.data[6] = (unsigned char)getReflex();

        can_data.data[7] = 0;    // dummy byte ... not used !!!

        can_send_message(&can_data);

        _delay_ms(500);

    }

    return 0;
}

uint32_t getDistance (unsigned char sensor_num) {
    if (sensor_num > 1) return ERROR_INVAL_NUM;

    // initialize the data request frame
    unsigned char sdata[32] = {
                                    0x24, 0x00, 0x01, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x0f, 0x00, 0x2E, 0x3B
                                };

    // send the request frame
    if (sensor_num == 0) uart_write((unsigned char*)sdata, 32);
    else uart1_write((unsigned char*)sdata, 32);

    // read the incomming data frame
    unsigned char buffer[68] = { 0 };

    if (sensor_num == 0) {
        if (uart_read((unsigned char*)buffer, 68) != 0) return ERROR_NO_TARGET;
    }
    else {
        if (uart1_read((unsigned char*)buffer, 68) != 0) return ERROR_NO_TARGET;
    }

    /*// calculating checksum
    uint16_t checksum = buffer[0];
    for (int i = 0; i < 31; i++) { checksum ^= buffer[i+1]; }*/

    //for (int i = 0; i < 68; i++) uart1_putc(buffer[i]);

    uint32_t distance = 0;
    for (int i = 0; i < 4; i++) distance |= ((uint32_t)buffer[36+i] << 8*i);

    if(distance == ERROR_OVERFLOW_IN) distance = ERROR_OVERFLOW;

    eeprom_write_block(buffer, (unsigned char*)100, 68);

     // clearing the uart buffers
    if (sensor_num == 0) uart_clear();
    else uart_clear();

    return distance;
}

bool getReflex() {
    /// not valid yet !
    return false;
}
