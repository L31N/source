
// cmpsSensor.cpp

#include <limits>

#include "cmpsSensor.h"

CmpsSensor::CmpsSensor(std::string ipcName, std::string canMember) : Sensor(ipcName, canMember) {
    angle = std::numeric_limits<unsigned int>::min();
}

CmpsSensor::~CmpsSensor() {}

unsigned int CmpsSensor::getAngle() {
    if (can->checkForNewData(canMember)) angle = int(*can->getValue(canMember));
    debug->send("Angle: %i", angle);
    return angle;
}
