
// laserSensor.cpp


#include "laserSensor.h"

const unsigned short error_value_distance = std::numeric_limits<unsigned int>::max();

LaserSensor::LaserSensor(const std::string ipcName, const std::string canName, const unsigned short _number) : CANSensor(ipcName, canName) {
    mmDistance = error_value_distance;
    number = _number;
}

LaserSensor::~LaserSensor() {}

unsigned int LaserSensor::getDistance(Unit unit) {
    if (can->checkNew()) {
        CAN::can_t frame;
        can->read(frame);
        mmDistance = 0;
        if (number % 2 == 0) for (int i = 0; i < 3; i++) mmDistance |= (frame.data[i] << 8*i);
        else for (int i = 0; i < 3; i++) mmDistance |= (frame.data[i+3] << 8*i);
    }

    switch (unit) {
        case mm:
            debug->send("Distance: %i mm", mmDistance);
            return mmDistance;

        case cm:
            debug->send("Distance: %i cm", mmDistance/10);
            return mmDistance/10;

        default:
            debug->send("ERROR: unknown unit: %i", unit);
            return mmDistance/10;
    }
}
