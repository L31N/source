
#ifndef _CAN_SENSOR_H_
#define _CAN_SENSOR_H_

#include <string>

#include "sensor.h"

#include "can/bbcan.h"
#include "debug/debug.h"

class CANSensor : public Sensor {
    public:
        CANSensor(const std::string _ipcName, const std::string _canMember);
        virtual ~CANSensor();

    protected:
        std::string canMember;
        CAN* can;
};

#endif // _CAN_SENSOR_H_
