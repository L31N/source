
// irSensor.cpp

#include "irSensor.h"

IRSensor::IRSensor(std::string ipcName, std::string canMember, Angle _angle) : DigitalSensor(ipcName, canMember) {
    angle = _angle;
}

IRSensor::~IRSensor() {}
