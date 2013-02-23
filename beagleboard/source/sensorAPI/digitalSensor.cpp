
// digitalSensor.cpp

#include "digitalSensor.h"

DigitalSensor::DigitalSensor(const std::string _ipcName, const std::string _canMember) : CANSensor(_ipcName, _canMember) {
    status = false;
}

DigitalSensor::~DigitalSensor() {}

bool DigitalSensor::getStatus() {
    if (can->checkForNewData(canMember)) status = can->getValue(canMember);
    debug->send("IRStatus: %i", status);
    return status;
}
