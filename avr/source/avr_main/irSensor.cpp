
// irSensor.cpp

#include "irSensor.h"

IRSensor::IRSensor() {
    irValues = malloc (NUM_OF_SENSORS * sizeof(bool));
}

IRSensor::~IRSensor() {
    free (irValues);
}

bool IRSensor::getStatus(unsigned char num) {
    if (can_check_message()) {
        can_t ir_frame = can_get_message(&ir_frame);

        /// SENSORS 0 - 6
        for (int i = 0; i < 7; i++) {
            irValues[i] = ir_frame.data[0] >> i & 0x01;
        }

        /// SENSORS 12 - 15
        for (int i = 7; i < NUM_OF_SENSORS; i++) {
            irValues[i] = ir_frame.data[1] >> i & 0x01;
        }
    }

    return ~(irValues[num]);
}
