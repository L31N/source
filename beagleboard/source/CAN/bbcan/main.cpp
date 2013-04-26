
#include <iostream>
#include <string>

#include "bbcan.h"

int main () {
    CAN motor0("MOTOR0");

    std::cout << "waiting for messages " << std::endl;

    while(!motor0.checkNew()) {
        std::cout << "." << std::flush;
        sleep(1);
    }

    CAN::can_t msg;
    msg.rtr = 0;
    msg.length = 8;

    msg.data[0] = 'F';
    msg.data[1] = 'F';
    msg.data[2] = 'F';
    msg.data[3] = 'F';
    msg.data[4] = 'F';
    msg.data[5] = 'F';
    msg.data[6] = 'F';
    msg.data[7] = 'F';

    if (motor0.read(msg)) {
        std::cout << "\nreading message now" << std::endl;
        std::cout << "message length: " << int(msg.length) << std::endl;
        for (unsigned int i = 0; i < msg.length; i++) std::cout << "[" << i << "]: " << msg.data[i] << std::endl;
    }
    else std::cout << "error while reading message" << std::endl;

    return 0;
}
