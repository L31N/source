
// cmpsSensor.cpp

#include <limits>

#include "cmpsSensor.h"

const unsigned int error_value_angle = std::numeric_limits<unsigned int>::max();

CmpsSensor::CmpsSensor(std::string ipcName, std::string canMember) : Sensor(ipcName, canMember) {
    angle = error_value_angle;
}

CmpsSensor::~CmpsSensor() {}

unsigned int CmpsSensor::getAngle() {
    if (can->checkForNewData(canMember)) angle = int(*can->getValue(canMember));
    debug->send("Angle: %i", angle);
    return angle;
}
