
#include <string>
#include <iostream>

#include "sensor/cmpsSensor.h"


int main () {
    std::cout << "starting programm " << std::endl;
    CmpsSensor cmps("CMPS", 0x60, "/dev/i2c-2");
    std::cout << "before getAngle()" << std::endl;
    //std::cout << "cmps: " << cmps->getAngle() << std::endl;

    return 0;
}
