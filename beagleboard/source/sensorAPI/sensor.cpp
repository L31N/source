
// sensor.cpp

#include "sensor.h"

Sensor::Sensor(const std::string _ipcName, const std::string _canMember) {
    ipcName = _ipcName;
    canMember = _canMember;
    debug = new Debug(ipcName);
    can = new CAN(ipcName);
    can->init_member(canMember);
}

Sensor::~Sensor() {
    delete can;
    delete debug;
}
