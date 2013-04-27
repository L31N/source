
// sensor.cpp

#include "sensor.h"

Sensor::Sensor(const std::string ipcName) {
    debug = new Debug(ipcName);
}

Sensor::~Sensor() {
    delete debug;
}
