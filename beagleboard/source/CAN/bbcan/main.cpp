
#include <string>
#include <iostream>

#include "can/bbcan.h"

/*int main () {
    CAN* can = new CAN("MOTION_CONTROLLER");
    can->init_member("LASER_SENSOR2");

    sleep(1);

    //can->setValue("MOTOR_2", (char*)"hello123");
    while(true) {
        char* buffer;
        if (can->checkForNewData("LASER_SENSOR2")) {
            buffer = can->getValue("LASER_SENSOR2");
            for (int i = 0; i < 8; i++) std::cout << "data[" << i << "]: " << int(buffer[i]) << std::endl;
        }
    }

    sleep(10);

    return 0;
}*/

int main () {
    CAN* can = new CAN("MOTION_CONTROLLER");
    can->init_member("MOTOR0");

    can->setValue("MOTOR0", 129);

    sleep(1);
    return 0;
}
