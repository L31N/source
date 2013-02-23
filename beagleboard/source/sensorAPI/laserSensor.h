
#ifndef _LASER_SENSOR_H_
#define _LASER_SENSOR_H_

#include <string>
#include <limits>

#include "canSensor.h"

extern const unsigned int error_value_distance;

class LaserSensor : public CANSensor {
    public:
        enum Direction {front, right, back, left};
        enum Unit {mm, cm};

        LaserSensor(const std::string ipcName, const std::string canMember, Direction _direction);
        ~LaserSensor();

        unsigned short getDistance(Unit unit = mm);

    private:
        Direction direction;
        unsigned short mmDistance;
};

#endif // _LASER_SENSOR_H_
