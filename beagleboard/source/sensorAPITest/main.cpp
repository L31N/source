
#include <string>
#include <iostream>

#include "sensor/cmpsSensor.h"
#include "sensor/laserSensor.h"
#include "irSensor.h"

int main () {
    std::cout << "starting programm " << std::endl;
    /*CmpsSensor* cmps = new CmpsSensor("CMPS", 0x60, "/dev/i2c-2");
    std::cout << "before getAngle()" << std::endl;
    std::cout << "cmps: " << cmps->getAngle() << std::endl;*/

    irSensor bsensor = new irSensor("BALL_SENSOR0", "BALL_SENSOR0", irSensor::G0);
    sleep(1);
    while(true) {
        unsigned int status = bsensor->getStatus();
        std::cout << "status: " << dist << std::endl;
        sleep();
    }
    //delete cmps;
    return 0;
}
