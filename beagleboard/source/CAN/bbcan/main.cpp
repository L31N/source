
#include <string>
#include <iostream>

#include "bbcan.h"

int main () {
    CAN* can = new CAN("MOTION_CONTROLLER");
    can->init_member("LASER_SENSOR2");

    sleep(1);

    //can->setValue("MOTOR_2", (char*)"hello123");
    while(true) {
        char* buffer;
        if (can->checkForNewData() {
            buffer = can->getValue("CAN_TEST_BOARD");
            std::cout << "data: " << buffer << std::endl;
        }
    }

    sleep(10);

    return 0;
}
