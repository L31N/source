
// sensor.cpp

#include "canSensor.h"

CANSensor::CANSensor(const std::string _ipcName, const std::string _canMember) : Sensor(_ipcName) {
    canMember = _canMember;
    can = new CAN(ipcName);
    can->init_member(canMember);
}

CANSensor::~CANSensor() {
    delete can;
}
