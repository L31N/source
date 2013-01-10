
// irSensor.cpp

#include "irSensor.h"

IRSensor::IRSensor(const std::string ipcName, const std::string canMember, Angle _angle) : DigitalSensor(ipcName, canMember) {
    angle = _angle;
}

IRSensor::~IRSensor() {}
