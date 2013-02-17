
#ifndef _SENSOR_H_
#define _SENSOR_H_

#include <string>

#include "sensor.h"

#include "can/bbcan.h"
#include "debug/bbdebug.h"

class Sensor {
    public:
        Sensor(const std::string ipcName, const std::string _canMember);
        virtual ~Sensor();

    protected:
        std::string ipcName;
        std::string canMember;
        CAN* can;
        Debug* debug;
};

#endif // _SENSOR_H_
