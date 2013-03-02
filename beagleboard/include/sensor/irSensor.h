
#ifndef _IR_SENSOR_H_
#define _IR_SENSOR_H_

#include <string>

#include "digitalSensor.h"

class IRSensor : public CANSensor {
    public:
        /*enum Angle {
            G0      = 0,
            G22_5   = 22,
            G45     = 45,
            G67_5   = 68,
            G90     = 90,
            G112_5  = 113,
            G135    = 135,
            G157_5  = 158,
            G180    = 180,
            G202_5  = 203,
            G225    = 225,
            G247_5  = 248,
            G270    = 270,
            G292_5  = 293,
            G315    = 315,
            G337_5  = 338,
        };*/

        IRSensor(const std::string ipcName, const std::string canMember);
        ~IRSensor();

        bool getStatus(unsigned char num);
        unsigned char getBallNum(unsigned char num_of_sensors = 22);

    private:
        unsigned int values;
};

#endif // _IR_SENSOR_H_

