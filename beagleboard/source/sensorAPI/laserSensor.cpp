
// laserSensor.cpp


#include "laserSensor.h"

const unsigned int error_value_distance = std::numeric_limits<unsigned int>::max();

LaserSensor::LaserSensor(const std::string ipcName, const std::string canMember, Direction _direction) : CANSensor(ipcName, canMember) {
    direction = _direction;
    mmDistance = error_value_distance;
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
