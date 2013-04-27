
// sensor.cpp

#include "canSensor.h"

CANSensor::CANSensor(const std::string ipcName, const std::string canName) : Sensor(ipcName) {
    can = new CAN(canName);
}

CANSensor::~CANSensor() {
    delete can;
}
