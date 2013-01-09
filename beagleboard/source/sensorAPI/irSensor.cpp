
// irSensor.cpp

#include "irSensor.h"

IRSensor::IRSensor(std::string ipcName, std::string canMember, Angle _angle) : Sensor(ipcName, canMember) {
    angle = _angle;
    status = false;
}

IRSensor::~IRSensor() {}

bool IRSensor::getStatus() {
    if (can->checkForNewData(canMember)) status = can->getValue(canMember);
    debug->send("IRStatus: %i", status);
    return status;
}
