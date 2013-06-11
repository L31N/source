
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
            if (datastr[0] == 0) {
                Vector vector(datastr.substr(1, 16));

                short rotationSpeed = 0;
                memcpy(&rotationSpeed, datastr.substr(17, 2).c_str(), sizeof(rotationSpeed));

                thDrive = new boost::thread(MotionControl::thDrive_fctn, extMtnCtrlr, vector, rotationSpeed);
                thDrive->detach();
            }
            else if (datastr[0] == 1) {
            }
            else if (datastr[0] == 2) {
            }
            else if (datastr[0] == 3) {
            }
            else if (datastr[0] == 4) {
            }
            else if (datastr[0] == 5) {
            }
            else if (datastr[0] == 6) {
            }
            else if (datastr[0] == 7) {
            }

            else {  /// unknown command byte
                std::cerr << "error: received unknown command-byte ..." << std::endl;
            }

        }
        else {
            // idle to avoid a cpu-time loss from prescaler
            usleep(500);
        }
    }
}

 void MotionControl::thDrive_fctn(ExtendedMotionController* emCtrlr, Vector vector, short rotationSpeed) {
    emCtrlr->drive(vector, rotationSpeed);
    while(42) sleep(1);
 }

 void MotionControl::thMoveto_fctn(ExtendedMotionController* emCtrlr, Vector vector, unsigned char speed, Vector dir) {

 }

 void MotionControl::thMove_fctn(ExtendedMotionController* emCtrlr, Vector vector, unsigned char speed, Vector dir) {

 }

 void MotionControl::thTurnto_fctn(ExtendedMotionController* emCtrlr, Vector dir, unsigned char speed, ExtendedMotionController::Direction turndir) {

 }

 void MotionControl::thTurn_fctn(ExtendedMotionController* emCtrlr, Vector dir, unsigned char speed, ExtendedMotionController::Direction turndir) {

 }

 void MotionControl::thPBreak_fctn(ExtendedMotionController* emCtrlr) {

 }

 void MotionControl::thIdle_fctn(ExtendedMotionController* emCtrlr) {

 }

 void MotionControl::thTest_fctn(ExtendedMotionController* emCtrlr) {

 }

 void MotionControl::killThreads() {
    delete thDrive;
    delete thMoveto;
    delete thMove;
    delete thTurnto;
    delete thTurn;
    delete thPBreak;
    delete thIdle;
    delete thTest;
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


