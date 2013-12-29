
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/delay.h>

#include "can.h"
#include "uart.h"

#include "adc.h"


const uint32_t ERROR_OVERFLOW_IN = 4294967293UL;
const uint32_t ERROR_OVERFLOW = 0xAAAAAAAD;
const uint32_t ERROR_NO_TARGET = 0xAAAAAAAE;
const uint32_t ERROR_INVAL_NUM = 0xAAAAAAAC;
const uint32_t ERROR_INVAL_CHECKSUM = 0xAAAAAAAB;
const uint32_t ERROR_NOT_READY = 0xAAAAAAA9;

const unsigned char CANID = 68;     // WENGLORS0
//const unsigned char CANID = 100;     // WENGLORS1

const unsigned int frame_length = 68;

void requestDistanceRS232 (unsigned char sensor_num);       // needs called the uart_init() function !!!
bool checkReadyRS232 (unsigned char sensor_num);            // needs called the uart_init() function !!!
uint32_t readDistanceRS232 (unsigned char sensor_num);      // needs called the uart_init() function !!!
uint16_t getDistanceADC (unsigned char sensor_num);         // needs called adc_init(); function !!!
bool getReflex();       /// not valid yet !!!

int main(void) {

    DDRE &= ~(1 << 4);  // set the reflex pin as input

    can_init(BITRATE_10_KBPS);
    sei();

    uart_init(38400);
    uart1_init(38400);

    adc_init();

    can_t can_data;
    can_data.id = CANID;
    can_data.flags.rtr = false;
    can_data.length = 7;


    uint32_t distance[2] = { 0 };

    uint32_t distance_rs232[2] = { 0 };
    uint16_t distance_adc[2] = { 0 };

    float c[2] = { 1 };  // the correction factor

    bool requested[2] = { false };

    /** ONLY TESTING ZONE HERE **/

    /** TESTING ZONE END HERE  **/

    /*while(true) {

        // read the distance laser sensors
        distance[0] = getDistance(0);
        distance[1] = getDistance(1);
        //distance[0] = 42;
        //distance[1] = 21;

        //for (int i = 0; i < 3; i++) uart1_putc((distance[0] >> i*8) & 0xFF);
        //uart1_putc(0xAA);
        for (int i = 0; i < 3; i++) can_data.data[i] = ( distance[0] >> i*8 ) & 0xFF;
        for (int i = 0; i < 3; i++) can_data.data[i+3] = ( distance[1] >> i*8 ) & 0xFF;

        // fill in the digital sensor value
        can_data.data[6] = (unsigned char)getReflex();
        //uart1_putc(can_data.data[6]);

        can_data.data[7] = 0;    // dummy byte ... not used !!!

        can_send_message(&can_data);

        _delay_ms(500);

    }*/

    while(42) {
        /// SENSOR_0
        distance_adc[0] = getDistanceADC(0);
        if (checkReadyRS232(0)) {   // digital value already pending
            requested[0] = false;
            distance_rs232[0] = readDistanceRS232(0);
            c[0] = distance_rs232[0] / distance_adc[0]; // recalculate correction

            distance[0] = distance_rs232[0];
        }
        else distance[0] = c[0] * distance_adc[0];

        if (!requested[0]) {
            requestDistanceRS232(0);
            requested[0] = true;
        }

        /// SENSOR_1
        distance_adc[1] = getDistanceADC(1);
        if (checkReadyRS232(1)) {   // digital value already pending
            requested[1] = false;
            distance_rs232[1] = readDistanceRS232(1);
            c[1] = distance_rs232[1] / distance_adc[1]; // recalculate correction

            distance[1] = distance_rs232[1];
        }
        else distance[1] = c[1] * distance_adc[1];

        if (!requested[1]) {
            requestDistanceRS232(1);
            requested[1] = true;
        }


        /// can posting
        for (int i = 0; i < 3; i++) can_data.data[i] = ( distance[0] >> i*8 ) & 0xFF;
        for (int i = 0; i < 3; i++) can_data.data[i+3] = ( distance[1] >> i*8 ) & 0xFF;
        can_data.data[6] = (unsigned char)getReflex();
        can_data.data[7] = 0;    // dummy byte ... not used !!!

        can_send_message(&can_data);

        _delay_ms(100);

    }

    return 0;
}

void requestDistanceRS232(unsigned char sensor_num) {
    // initialize the data request frame
    unsigned char sdata[32] = {
                                    0x24, 0x00, 0x01, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x0f, 0x00, 0x2E, 0x3B
                                };

    // send the request frame
    if (sensor_num == 0) uart_write(sdata, 32);
    else uart1_write(sdata, 32);

    return;
}

bool checkReadyRS232(unsigned char sensor_num) {
    if (sensor_num == 0 && uart_count() >= frame_length) return true;
    else if (sensor_num == 1 && uart1_count() >= frame_length) return true;
    else return false;
}

uint32_t readDistanceRS232(unsigned char sensor_num) {
    if (sensor_num > 1) return ERROR_INVAL_NUM;
    else if (!checkReadyRS232(sensor_num)) return ERROR_NOT_READY;

    else {  // all fine --> read and analyze the uart-buffer contents
         unsigned char buffer[500] = {0};
        if (sensor_num == 0) uart_read(buffer, uart_count());
        else uart1_read(buffer, uart_count());

        unsigned int i = 0;
        uint32_t distance = 0x0;

        while (i < 500) {

            for (; i < sizeof(buffer) && buffer[i] != 0x24; i++);

            unsigned short protlen = (buffer[i + 4]);
            protlen |= (buffer[i+5] << 8);

            unsigned short stop = (buffer[i + protlen - 2] << 8) | (buffer[i + protlen - 1]);
            if (stop == 0x2e3b) {   // could be an valid frame
                unsigned short check = 0x24;

                for (unsigned int j = 1; j < protlen - 5;j++ ) check ^= buffer[i+j];

                //unsigned short bcheck = (buffer[i + protlen - 4]) | (buffer[i + protlen - 3] << 8);
                //if (bcheck == check) {
                if (42) {
                    // extract the distance information
                    //distance = 0xAA;
                    for (int l = 0; l < 4; l++) distance |= (buffer[i + l + 36] << 8*l);
                    break;
                }
                else i++;
            }
            else i++;
        }
        return distance;
    }
}

uint16_t getDistanceADC(unsigned char sensor_num) {
    if (sensor_num > 1) return (uint16_t)ERROR_INVAL_NUM;

    unsigned short adc_value = adc_read(sensor_num);
    float distance = adc_value * 3.5934 + 59.1305;

    return uint16_t (distance);
}

bool getReflex() {
    return !bool((PINE & (1 << 4)));
}

/*uint32_t getDistanceRS232 (unsigned char sensor_num) {
    if (sensor_num > 1) return ERROR_INVAL_NUM;

    // initialize the data request frame
    unsigned char sdata[32] = {
                                    0x24, 0x00, 0x01, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x0f, 0x00, 0x2E, 0x3B
                                };

    // send the request frame
    if (sensor_num == 0) uart_write(sdata, 32);
    else uart1_write(sdata, 32);

    _delay_ms(500);

    // read the frame
    unsigned char buffer[500] = {0};
    if (sensor_num == 0) uart_read(buffer, uart_count());
    else uart1_read(buffer, uart_count());

    unsigned int i = 0;
    uint32_t distance = 0x0;

    while (i < 500) {

        //uart1_putc('*');

        for (; i < sizeof(buffer) && buffer[i] != 0x24; i++);

        unsigned short protlen = (buffer[i + 4]);
        protlen |= (buffer[i+5] << 8);

        unsigned short stop = (buffer[i + protlen - 2] << 8) | (buffer[i + protlen - 1]);
        if (stop == 0x2e3b) {   // could be an valid frame
            unsigned short check = 0x24;

            for (unsigned int j = 1; j < protlen - 5;j++ ) check ^= buffer[i+j];
            unsigned short bcheck = (buffer[i + protlen - 4]) | (buffer[i + protlen - 3] << 8); */

            /*uart1_putc('|');
            uart1_putc(bcheck);
            uart1_putc(bcheck >> 8);
            uart1_putc('=');
            uart1_putc(check);
            uart1_putc(check >> 8);
            uart1_putc('|');*/

            //if (bcheck == check) {
            /*if (42) {
                // extract the distance information
                //distance = 0xAA;
                for (int l = 0; l < 4; l++) distance |= (buffer[i + l + 36] << 8*l);
                break;
            }
            else i++;
        }
        else i++;

        //uart1_putc('#');
    }

    //uart1_putc('%');

    return distance;
}*/