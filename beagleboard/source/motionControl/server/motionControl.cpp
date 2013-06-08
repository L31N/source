
// motionControl.cpp

#include "motionControl.h"

MotionControl::MotionControl() {
    ipcRcon = new ipcReceivingConnection("MOTION_CONTROL", 1, 34);
    extMtnCtrlr = new ExtendedMotionController();

    //thReceive = new boost::thread(MotionControl::thReceive_fctn, ipcRcon);
    //thControl = new boost::thread(MotionControl::thControl_fctn, extMtnCtrlr);

    dbg = new Debug("MOTION_CONTROL_DBG");
}

MotionControl::~MotionControl() {
    delete thDrive;
    delete thMoveto;
    delete thMove;
    delete thTurnto;
    delete thTurn;
    delete thPBreak;
    delete thIdle;
    delete thTest;

    delete extMtnCtrlr;
    delete ipcRcon;

    delete dbg;
}

void MotionControl::run() {
    while(42) {
        if (ipcRcon->checkForNewData()) {   // handle new data
            std::string datastr = ipcRcon->readDataFromBuffer()->getData();

        }
        else {
            // idle to avoid a cpu-time loss from prescaler
            usleep(500);
        }
    }
}

/*void MotionControl::thReceive_fctn(ipcReceivingConnection* rcon) {
    while(42) {
        if (rcon->checkForNewData()) {
            std::string datastr = rcon->readDataFromBuffer()->getData();

            // parse the datastr
            if (datastr[0] == 0) {          // drive();
                std::string strVector = datastr.substr(1, 16+1);
                std::string strRotSpeed = datastr.substr(17, 18+1);

                Vector vector(strVector);

                short rotationSpeed = 0;
                memcpy(&rotationSpeed, strRotSpeed.c_str(), 2);

                /// call fctn !!!
            }
            else if (datastr[0] == 1) {     // moveto();
                std::string strVector = datastr.substr(1, 16+1);
                std::string strSpeed = datastr.substr(17, 18);
                std::string strDir = datastr.substr(18, 33+1);

                Vector vector(strVector);
                Vector dir(strDir);

                unsigned char speed = 0;
                memcpy(&speed, strSpeed.c_str(), 1);


            }
            else if (datastr[0] == 2) {     // move();
                std::string strVector = datastr.substr(1, 16+1);
                std::string strSpeed = datastr.substr(17, 18);
                std::string strDir = datastr.substr(18, 33+1);

                Vector vector(strVector);
                Vector dir (strDir);

                unsigned char speed = 0;
                memcpy(&speed, strSpeed, 1);
            }
            else if (datastr[0] == 3) {     // turnto();
                std::string strVector = datastr.substr(1, 16+1);
                std::string strSpeed = datastr.substr(17, 18);
                std::stirng strTurndir = datastr.substr(18, 19);

                Vector vector(strVector);

                unsigned char speed = 0;
                memcpy(&speed, strSpeed.c_str(), 1);

                unsigned char turndir = 0;
                memcpy(&turndir, strTurndir, 1);
            }
            else {
                dbg->send("could not resolve command-byte: %d", datastr[0]);
            }
        }
        else usleep(200);
    }
}

void MotionControl::thControl_fctn(ExtendedMotionController* emctrlr) {

}*/


