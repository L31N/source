
#include <string>
#include <iostream>

#include "sensor/cmpsSensor.h"


int main () {
    std::cout << "create cmps ..." << std::endl;
    CmpsSensor* cmps = new CmpsSensor("CMPS", 0x60, "/dev/i2c-2");

    std::cout << "start to read cmps ... " << std::endl;
    std::cout << "cmps: " << cmps->getAngle() << std::endl;

    return 0;
}
