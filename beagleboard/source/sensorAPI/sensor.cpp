
// sensor.cpp

#include "sensor.h"

Sensor::Sensor(const std::string _ipcName) {
    std::cout << "sensor::sensor()" << std::endl;

    ipcName = _ipcName;
    debug = new Debug(ipcName);
}

Sensor::~Sensor() {
    delete debug;
}
