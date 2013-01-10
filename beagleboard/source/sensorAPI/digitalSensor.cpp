
// digitalSensor.cpp

#include "digitalSensor.h"

DigitalSensor::DigitalSensor(std::string ipcName, std::string canMember) : Sensor(ipcName, canMember) {
    status = false;
}

DigitalSensor::~DigitalSensor() {}

bool DigitalSensor::getStatus() {
    if (can->checkForNewData(canMember)) status = can->getValue(canMember);
    debug->send("IRStatus: %i", status);
    return status;
}
