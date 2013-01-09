
#ifndef _CMPS_SENSOR_H_
#define _CMPS_SENSOR_H_

#include "sensor.h"

class CmpsSensor : public Sensor {
    public:
        CmpsSensor(std::string ipcName, std::string canMember);
        ~CmpsSensor();

        unsigned int getAngle();

    private:
        unsigned int angle;
};

#endif // _CMPS_SENSOR_H_
