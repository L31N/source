
// motionControl.cpp

#include "motionControl.h"

MotionControl::MotionControl() {
    ipcRcon = new ipcReceivingConnection("MOTION_CONTROL", 1, 32);
    extMtnCtrlr = new ExtendedMotionController();

    thReceive = new boost::thread(MotionControl::thReceive_fctn, ipcRcon);
    thControl = new boost::thread(MotionControl::thControl_fctn, extMtnCtrlr);
}

MotionControl::~MotionControl() {
    delete thReceive;
    delete thControl;

    delete extMtnCtrlr;
    delete ipcRcon;
}

void MotionControl::run() {
    thReceive->detach();
    thControl->detach();

    while(true) sleep(1);
}

void MotionControl::thReceive_fctn(ipcReceivingConnection* rcon) {

}

void MotionControl::thControl_fctn(ExtendedMotionController* emctrlr) {

}
