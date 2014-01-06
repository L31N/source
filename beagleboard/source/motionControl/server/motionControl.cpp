
// motionControl.cpp

#include "motionControl.h"

MotionControl::MotionControl() {
    ipcRcon = new ipcReceivingConnection("MOTION_CONTROL", 1, 34);
    extMtnCtrlr = new ExtendedMotionController();

    gyro = new GyroSensor("GYRO");

    thDrive = NULL;
    thMoveto = NULL;
    thMove = NULL;
    thTurnto = NULL;
    thTurn = NULL;
    thPBreak = NULL;
    thIdle = NULL;
    thTest = NULL;

    dbg = new Debug("MOTION_CONTROL_DBG");
}

MotionControl::~MotionControl() {
    killThreads();

    delete extMtnCtrlr;
    delete ipcRcon;

    delete gyro;

    delete dbg;
}

void MotionControl::run() {
    while(42) {
        if (ipcRcon->checkForNewData()) {
            stopThreads();
            std::string datastr = ipcRcon->readDataFromBuffer()->getData();

            if (datastr[0] == 0) {
                Vector vector(datastr.substr(1, 16));

                short rotationSpeed = 0;
                memcpy(&rotationSpeed, datastr.substr(17, 2).c_str(), sizeof(rotationSpeed));

                thDrive = new boost::thread(MotionControl::thDrive_fctn, extMtnCtrlr, vector, rotationSpeed, gyro);
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
                thPBreak = new boost::thread(MotionControl::thPBreak_fctn, extMtnCtrlr);
            }
            else if (datastr[0] == 6) {
            }
            else if (datastr[0] == 7) {
            }

            else {  /// unknown command byte
                std::cerr << "error: received unknown command-byte ..." << std::endl;
            }
        }
    }
}

 void MotionControl::thDrive_fctn(ExtendedMotionController* emCtrlr, Vector vector, short rotationSpeed, GyroSensor* gyro) {
    std::cout << "drive(Vector(" << vector.getX() << ", " << vector.getY() << "), " << rotationSpeed << ")" << std::endl;

    double old_angle = vector.getAngle(true, false);
    std::cout << "old: " << old_angle << std::endl;
    while(true) {
        try {
            double angle = gyro->getVector().getAngle(true, false);
            std::cout << "angle: " << angle << std::endl;
            vector.setAngle(old_angle - angle, true, false);   // update the vector to drive to dependent to turnangle
            vector.print();

            emCtrlr->drive(vector, rotationSpeed);
            boost::this_thread::sleep_for(boost::chrono::milliseconds(5));
        }
        catch (boost::thread_interrupted&) {
            return;
        }
    }
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
        std::cout << "pbreak()" << std::endl;
        while (true) {
            emCtrlr->pbreak();
            boost::this_thread::sleep_for(boost::chrono::milliseconds(5));
        }
 }

 void MotionControl::thIdle_fctn(ExtendedMotionController* emCtrlr) {

 }

 void MotionControl::thTest_fctn(ExtendedMotionController* emCtrlr) {

 }

void MotionControl::stopThreads() {
    if (thDrive != NULL) {
        thDrive->interrupt();
        thDrive->join();
        delete thDrive;
        thDrive = NULL;
    }

    if (thMoveto != NULL) {
        thMoveto->interrupt();
        thMoveto->join();
        delete thMoveto;
        thDrive = NULL;
    }

    if (thMove != NULL) {
        thMove->interrupt();
        thMove->join();
        delete thMove;
        thMove = NULL;
    }

    if (thTurnto != NULL) {
        thTurnto->interrupt();
        thTurnto->join();
        delete thTurnto;
        thTurnto = NULL;
    }

    if (thTurn != NULL) {
        thTurn->interrupt();
        thTurn->join();
        delete thTurn;
        thTurn = NULL;
    }

    if (thPBreak != NULL) {
        thPBreak->interrupt();
        thPBreak->join();
        delete thPBreak;
        thPBreak = NULL;
    }

    if (thIdle != NULL) {
        thIdle->interrupt();
        thIdle->join();
        delete thIdle;
        thIdle = NULL;
    }

    if (thTest != NULL) {
        thTest->interrupt();
        thTest->join();
        delete thTest;
        thTest = NULL;
    }

    return;
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


