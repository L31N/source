
// sensor.cpp

#include "sensor.h"

Sensor::Sensor(const std::string _ipcName) {
    ipcName = _ipcName;
    debug = new Debug(ipcName);
}

Sensor::~Sensor() {
    delete debug;
}
