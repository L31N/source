
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

    char* data = new char[8];
    //char data[8];

    data[0] = 0x0F;
    data[1] = 0xF0;
    data[2] = 0xAA;
    data[3] = 0xFF;
    data[4] = 0xBC;
    data[5] = 0x42;
    data[6] = 0x12;
    data[7] = 0x21;

    can->setValue("MOTOR0", data);

    sleep(1);
    return 0;
}
