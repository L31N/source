
#include <string>
#include <iostream>

#include "sensor/cmpsSensor.h"
#include "sensor/laserSensor.h"
#include "sensor/irSensor.h"

int main () {
    std::cout << "starting programm " << std::endl;
    /*CmpsSensor* cmps = new CmpsSensor("CMPS", 0x60, "/dev/i2c-2");
    std::cout << "before getAngle()" << std::endl;
    std::cout << "cmps: " << cmps->getAngle() << std::endl;*/

    IRSensor* bsensor = new IRSensor("BALL_SENSOR0", "BALL_SENSOR0", IRSensor::G0);
    sleep(1);
    while(true) {
        unsigned int status = bsensor->getStatus();
        std::cout << "status: " << status << std::endl;
        sleep(1);
    }
    //delete cmps;
    return 0;
}
