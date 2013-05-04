
#ifndef _CAN_SENSOR_H_
#define _CAN_SENSOR_H_

#include <string>

#include "sensor.h"

#include "can/bbcan.h"
#include "debug/debug.h"

class CANSensor : public Sensor {
    public:
        CANSensor(const std::string ipcName, const std::string canName);        // ipcName is used to print debug-output; canName to identify for spi_mserver
        virtual ~CANSensor();

    protected:
        CAN* can;
};

#endif // _CAN_SENSOR_H_
