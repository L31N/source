
#ifndef _IR_SENSOR_H_
#define _IR_SENSOR_H_

#include <string>

#include "sensor.h"

class IRSensor : public Sensor {
    public:
        enum Angle {G0      = 0,
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
                    };

        IRSensor(std::string ipcName, std::string canMember, Angle _angle);
        ~IRSensor();

        bool getStatus();

    private:
        Angle angle;
        bool status;

};

#endif // _IR_SENSOR_H_
