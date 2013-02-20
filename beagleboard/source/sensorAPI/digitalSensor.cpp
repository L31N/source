
// digitalSensor.cpp

#include "digitalSensor.h"

DigitalSensor::DigitalSensor(const std::string ipcName, const std::string canMember) : CANSensor(ipcName, canMember) {
    status = false;
}

DigitalSensor::~DigitalSensor() {}

bool DigitalSensor::getStatus() {
    if (can->checkForNewData(canMember)) status = can->getValue(canMember);
    debug->send("IRStatus: %i", status);
    return status;
}
