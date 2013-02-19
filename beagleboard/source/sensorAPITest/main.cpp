
#include <string>
#include <iostream>

#include "sensor/cmpsSensor.h"


int main () {
    CmpsSensor* cmps = new CmpsSensor("CMPS", 0x60, "/dev/i2c-2");

    std::cout << "cmps: " << cmps->getAngle() << std::endl;

    return 0;
}
