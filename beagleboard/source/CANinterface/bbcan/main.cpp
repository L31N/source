
#include <string>
#include <iostream>

#include "bbcan.h"

int main () {
    CAN* can = new CAN("CAN_INTERFACE_TESTING");
    can->init_member("CAN_TEST_BOARD");

    sleep(1);

    //can->setValue("MOTOR_2", (char*)"hello123");
    while(true) {
        char* buffer;
        buffer = can->getValue("CAN_TEST_BOARD");
        if (buffer) std::cout << "data: " << buffer << std::endl;
    }

    sleep(10);

    return 0;
}
