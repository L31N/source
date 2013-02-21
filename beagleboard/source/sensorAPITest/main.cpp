
#include <string>
#include <iostream>

#include "sensor/cmpsSensor.h"


int main () {
    std::cout << "starting programm " << std::endl;
    /*CmpsSensor* cmps = new CmpsSensor("CMPS", 0x60, "/dev/i2c-2");
    std::cout << "before getAngle()" << std::endl;
    std::cout << "cmps: " << cmps->getAngle() << std::endl;*/

    LaserSensor* laser2 = new LaserSensor("LASER_SENSOR_2", "LASER_SENSOR_2", LaserSensor::back);
    sleep(1);
    unsigned int dist = laser2->getDistance(LaserSensor::mm);
    std::cout << "Distance: " << dist << std::endl;

    //delete cmps;
    return 0;
}
