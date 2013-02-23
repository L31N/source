
#ifndef _DIGITAL_SENSOR_H_
#define _DIGITAL_SENSOR_H_

#include <string>

#include "canSensor.h"

class DigitalSensor : public CANSensor {
    public:
        DigitalSensor(const std::string _ipcName, const std::string _canMember);
        ~DigitalSensor();

        bool getStatus();

    protected:
        bool status;

};

#endif // _DIGITAL_SENSOR_H_
