
#ifndef _LASER_SENSOR_H_
#define _LASER_SENSOR_H_

#include <string>
#include <limits>

#include "canSensor.h"

extern const unsigned short error_value_distance;

class LaserSensor : public CANSensor {
    public:
        enum Unit {mm, cm};

        LaserSensor(const std::string ipcName, const std::string canName, const unsigned short _number);
        ~LaserSensor();

        unsigned int getDistance(Unit unit = mm);

    private:
        unsigned int mmDistance;
        unsigned short number;
};

#endif // _LASER_SENSOR_H_
