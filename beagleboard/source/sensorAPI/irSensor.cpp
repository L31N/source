
// irSensor.cpp

#include "irSensor.h"

unsigned char IRSensor::values[8] = { 0 };

IRSensor::IRSensor(const std::string ipcName, const std::string canName, unsigned char num) : CANSensor(ipcName, canName) {
    number = num;
}

IRSensor::~IRSensor() {}

unsigned int IRSensor::getValue() {
    can_frame frame;
    if (can->read(frame)) {
        for (int i = 0; i < 8; i++) {
            IRSensor::values[i]  = frame.data[i];
        }
    }

    return IRSensor::values[number];
}
