
#include <iostream>
#include <string>

#include "bbcan.h"

int main () {
    CAN motor0("MOTOR0");

    CAN::can_t msg;
    msg.rtr = 0;
    msg.length = 8;

    msg.data[0] = 'E';
    msg.data[1] = 'N';
    msg.data[2] = 'I';
    msg.data[3] = 'G';
    msg.data[4] = 'M';
    msg.data[5] = 'A';
    msg.data[6] = '4';
    msg.data[7] = '2';

    motor0.write(msg);

    return 0;
}
