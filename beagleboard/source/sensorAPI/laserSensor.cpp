
// laserSensor.cpp

#include <limits>

#include "laserSensor.h"

LaserSensor::LaserSensor(std::string ipcName, std::string canMember, Direction _direction) : Sensor(ipcName, canMember) {
    direction = _direction;
    mmDistance = std::numeric_limits<unsigned int>::min();
}

LaserSensor::~LaserSensor() {}

unsigned int LaserSensor::getDistance(Unit unit) {
    if (can->checkForNewData(canMember)) mmDistance = int(*can->getValue(canMember));

    switch (unit) {
        case mm:
            debug->send("Distance: %i mm", mmDistance);
            return mmDistance;

        case cm:
            debug->send("Distance: %i cm", mmDistance/10);
            return mmDistance/10;

        default:
            debug->send("ERROR: LaserSensor(%i): unknown unit: %i", direction, unit);
            return mmDistance/10;
    }
}
