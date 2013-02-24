
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

        for (int j = 0; j <= NUM_OF_SENSORS / 8; j++) {
            for (int i = 0; i < 8 && (j*8+i) < NUM_OF_SENSORS; i++) {
                irValues[i] = (ir_frame.data[j] >> num) & 0x01;
            }
        }
    }
}
