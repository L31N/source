
#ifndef _DIGITAL_SENSOR_H_
#define _DIGITAL_SENSOR_H_

#include <string>

#include "sensor.h"

class DigitalSensor : public Sensor {
    public:
        DigitalSensor(std::string ipcName, std::string canMember);
        ~DigitalSensor();

        bool getStatus();

    protected:
        bool status;

};

#endif // _DIGITAL_SENSOR_H_
