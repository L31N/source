
#ifndef _IR_SENSOR_H_
#define _IR_SENSOR_H_

#include <string>

#include "digitalSensor.h"

class IRSensor : public CANSensor {
    public:
        IRSensor(const std::string ipcName, const std::string canName, unsigned char num);
        ~IRSensor();

        unsigned int getValue();

    private:
        unsigned char number;

        static unsigned char values[8];
};

#endif // _IR_SENSOR_H_

