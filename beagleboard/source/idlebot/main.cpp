
#include <iostream>
#include <signal.h>

#include "motion/motionController.h"
#include "bbcan.h"


const unsigned int LED_REMOTE_ID = 20;

int main () {
    MotionController mcontrol;
    mcontrol.pbreak();

    CAN can("");
    can.init_member("LED_REMOTE");

    char data[8];
    data[0] = 0xF0;
    data[1] = 0x00;

    for (int i = 0; i < 6; i++) data[i+2] = 0x00;

    can.setValue("LED_REMOTE", data);

    return 0;
}
