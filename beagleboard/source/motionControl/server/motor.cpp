
// motor.cpp

#include "motor.h"

Motors::Motors(const std::string _ipcName, const std::string _canName) {
    ipcName = _ipcName;
    canName = _canName;

    speeds = new short[4];
    for (unsigned char c = 0; c < 4; c++) speeds[c] = 0;

    can = new CAN(canName);
}

Motors::~Motors() {
    delete can;
}

void Motors::setSpeed(short speed0, short speed1, short speed2, short speed3) {
    speeds[0] = speed0;
    speeds[1] = speed1;
    speeds[2] = speed2;
    speeds[3] = speed3;

    CAN::can_t can_frame;
    can_frame.rtr = 0;
    can_frame.length = 8;

    for (int i = 0; i < 4; i++) {
        if (speeds[i] < 0) {
            can_frame.data[2*i] = 1;
            can_frame.data[2*i + 1] = speeds[i] * -1;
        }
        else {
            can_frame.data[2*i] = 0;
            can_frame.data[2*i + 1] = speeds[i];
        }
    }

    can->write(can_frame);

    return;
}

void Motors::getSpeed(short& speed0, short& speed1, short& speed2, short& speed3) {
    speed0 = speeds[0];
    speed1 = speeds[1];
    speed2 = speeds[2];
    speed3 = speeds[3];

    return;
}
