
#include <string>
#include <iostream>

#include "bbcan.h"

int main () {
    CAN* can = new CAN("CAN_INTERFACE_TESTING");
    can->init_member("MOTOR_2");

    can->setValue("MOTOR_2", (char*)"hello123");

    return 0;
}
