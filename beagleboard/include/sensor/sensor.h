
#ifndef _SENSOR_H_
#define _SENSOR_H_

#include <string>

#include "debug/bbdebug.h"

class Sensor {
    public:
        Sensor(const std::string _ipcName);
        virtual ~Sensor();

    protected:
        std::string ipcName;
        Debug* debug;
};

#endif // _SENSOR_H_
