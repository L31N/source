
#include <iostream>
#include <signal.h>

#include "motion/motionController.h"
#include "can/bbcan.h"

int main () {
    MotionController mcontrol;
    mcontrol.pbreak();

    CAN can("IDLE_BOT");
    can.init_member("LED0");

    char data[8];
    data[0] = 0xFF;

    for (int i = 0; i < 7; i++) data[i+1] = 0x00;

    can.setValue("LED0", data);

    return 0;
}
