
#ifndef _LASER_SENSOR_H_
#define _LASER_SENSOR_H_

#include <string>
#include <limits>

#include "sensor.h"

extern const unsigned int error_value_distance;

class LaserSensor : public Sensor {
    public:
        enum Direction {front, right, back, left};
        enum Unit {mm, cm};

        LaserSensor(std::string ipcName, std::string canMember, Direction _direction);
        ~LaserSensor();

        unsigned int getDistance(Unit unit = cm);

    private:
        Direction direction;
        unsigned int mmDistance;
};

#endif // _LASER_SENSOR_H_
