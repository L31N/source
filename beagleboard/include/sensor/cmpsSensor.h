
#ifndef _CMPS_SENSOR_H_
#define _CMPS_SENSOR_H_

#include <limits>
#include <string>

#include "i2cSensor.h"

extern const unsigned int error_value_angle;

class CmpsSensor : public I2CSensor {
    public:
        CmpsSensor(const std::string _ipcName, const unsigned char _i2c_addr, const std::string _i2c_device);
        ~CmpsSensor();

        unsigned int getAngle();

    private:
        unsigned int angle;
};

#endif // _CMPS_SENSOR_H_
