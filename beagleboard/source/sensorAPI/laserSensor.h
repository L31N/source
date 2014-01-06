
#ifndef _LASER_SENSOR_H_
#define _LASER_SENSOR_H_

#include <string>
#include <limits>

#include "canSensor.h"

extern const unsigned short error_value_distance;

class LaserSensor : public CANSensor {
    public:
        enum Unit {mm, cm};

        /**
        ipcName: name used for debug messages,
        canName: name which is connected with the can_id,
        num:     number of sensor on the specific board (wenglors0:num / wenglors1:num)
        **/

        LaserSensor(const std::string ipcName, const std::string canName, unsigned char num);
        ~LaserSensor();

        unsigned int getDistance(Unit unit = mm);

    private:
        unsigned int mmDistance;
        unsigned char number;

        static const unsigned short offset[4];
        //static const unsigned short offset0 = 100;
        //static const unsigned short offset1 = 100;
};

#endif // _LASER_SENSOR_H_
