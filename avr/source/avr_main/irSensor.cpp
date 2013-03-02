
// irSensor.cpp

#include <stdlib.h>
#include "irSensor.h"
#include "can.h"
#include "uart.h"
#include "board.h"


IRSensor::IRSensor() {
    irValues = 0;

    can_filter_t filter_ir;
    filter_ir.flags.rtr = 0;
    filter_ir.mask = 0x7E0;
    filter_ir.id = 129;

    can_set_filter(0, &filter_ir);
}

IRSensor::~IRSensor() {
}

bool IRSensor::getStatus(unsigned char num) {
    if (num > 10) return false;
    if (num > 5) num += 6;

    if (can_check_message()) {
        //uart_debug("new ir-can message");
        Board board;
        board.ledSwitch(2);
        can_t ir_frame;
        can_get_message(&ir_frame);

        irValues = 0;
        irValues |= ir_frame.data[0];
        irValues |= ((unsigned long)ir_frame.data[1] << 8);
        irValues |= ((unsigned long)ir_frame.data[2] << 16);
    }

    //uart_debug("irValues: %x\n\r", irValues);
    return (irValues >> num) & 0x01;
}

unsigned char IRSensor::getBallNum() {

    unsigned int tmp = 0;
    unsigned int maxConnected = 0;
    unsigned short max_angle_num = 0;

    for (int i = 0; i < NUM_OF_SENSORS; i++) {
        if(this->getStatus(i)) {
            tmp ++;
            if (i == NUM_OF_SENSORS - 1) {      // check for Zero-Intersection
                int j = 0;
                for (; this->getStatus(i); j++) tmp++;
                if (tmp > maxConnected) {
                    maxConnected = tmp;
                    max_angle_num = i + j;
                }
            }
            else {
                if (tmp > maxConnected) {
                    maxConnected = tmp;
                    max_angle_num = i;
                }
                tmp = 0;
            }
        }
    }

    max_angle_num = (max_angle_num + 1 - maxConnected + maxConnected/2) % (NUM_OF_SENSORS + 1);     // calculates the central angle of the "high-section".
    return max_angle_num;
}
