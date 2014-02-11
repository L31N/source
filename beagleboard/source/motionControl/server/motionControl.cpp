
// motionControl.cpp

#include "motionControl.h"

MotionControl::MotionControl() {
    ipcRcon = new ipcReceivingConnection("MOTION_CONTROL", 1, 34);
    extMtnCtrlr = new ExtendedMotionController();

    gyro = new GyroSensor("GYRO");

    thActive = NULL;

    dbg = new Debug("MOTION_CONTROL_DBG");
}

MotionControl::~MotionControl() {
    delete thActive;

    delete extMtnCtrlr;
    delete ipcRcon;

    delete gyro;

    delete dbg;
}

void MotionControl::run() {
    while(42) {
        //if (ipcRcon->checkForNewData()) {

        // blocks until new data in buffer and read them
        std::string datastr = ipcRcon->readDataFromBuffer()->getData();

        // stop the running thread
        if (thActive != NULL) {
            thActive->interrupt();
            thActive->join();
            delete thActive;
            thActive = NULL;

        }

        // extract data from ipc-string

        if (datastr[0] == 0) {  /// drive
            Vector vector(datastr.substr(1, 16));

            short rotationSpeed = 0;
            memcpy(&rotationSpeed, datastr.substr(17, 2).c_str(), sizeof(rotationSpeed));

            thActive = new boost::thread(MotionControl::thDrive_fctn, extMtnCtrlr, vector, rotationSpeed, gyro);
        }
        else if (datastr[0] == 1) {     /// moveto
            Vector vector(datastr.substr(1, 16));

            unsigned char speed = 0;
            memcpy(&speed, datastr.substr(17,1).c_str(), sizeof(speed));

            Vector dir(datastr.substr(18, 16));

            thActive = new boost::thread(MotionControl::thMoveto_fctn, extMtnCtrlr, vector, speed, dir, gyro);
        }
        else if (datastr[0] == 2) {     /// move
            Vector vector(datastr.substr(1, 16));

            unsigned char speed = 0;
            memcpy(&speed, datastr.substr(17,1).c_str(), sizeof(speed));

            Vector dir(datastr.substr(18, 16));

            thActive = new boost::thread(MotionControl::thMove_fctn, extMtnCtrlr, vector, speed, dir, gyro);
        }
        else if (datastr[0] == 3) {     /// turnto
            Vector vector(datastr.substr(1, 16));

            unsigned char speed = 0;
            memcpy(&speed, datastr.substr(17,1).c_str(), sizeof(speed));

            unsigned char turndir = 0;
            memcpy(&turndir, datastr.substr(18,1).c_str(), sizeof(turndir));

            thActive = new boost::thread(MotionControl::thTurnto_fctn, extMtnCtrlr, vector, speed, (ExtendedMotionController::Direction)turndir, gyro);
        }
        else if (datastr[0] == 4) {     /// turn
            Vector vector(datastr.substr(1, 16));

            unsigned char speed = 0;
            memcpy(&speed, datastr.substr(17,1).c_str(), sizeof(speed));

            unsigned char turndir = 0;
            memcpy(&turndir, datastr.substr(18,1).c_str(), sizeof(turndir));

            thActive = new boost::thread(MotionControl::thTurn_fctn, extMtnCtrlr, vector, speed, (ExtendedMotionController::Direction)turndir, gyro);
        }
        else if (datastr[0] == 5) {     /// pbreak
            thActive = new boost::thread(MotionControl::thPBreak_fctn, extMtnCtrlr);
        }
        else if (datastr[0] == 6) {     /// idle
            thActive = new boost::thread(MotionControl::thIdle_fctn, extMtnCtrlr);
        }
        else if (datastr[0] == 7) {     /// test
            thActive = new boost::thread(MotionControl::thTest_fctn, extMtnCtrlr);
        }

        else {  /// unknown command byte
            std::cerr << "error: received unknown command-byte ..." << std::endl;
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

            //vector.setAngle(vector, -5*rotationSpeed, true, false);

            //std::cout << "angle: " << angle << std::endl;
            vector.setAngle(old_angle - angle, true, false);   // update the vector to drive to dependent to turnangle
            //vector.print();

            emCtrlr->drive(vector, rotationSpeed);
            boost::this_thread::sleep_for(boost::chrono::milliseconds(8));
        }
        catch (boost::thread_interrupted&) {
            return;
        }
    }
 }

 void MotionControl::thMoveto_fctn(ExtendedMotionController* emCtrlr, Vector vector, unsigned char speed, Vector dir, GyroSensor* gyro) {

 }

 void MotionControl::thMove_fctn(ExtendedMotionController* emCtrlr, Vector vector, unsigned char speed, Vector dir, GyroSensor* gyro) {

 }

 void MotionControl::thTurnto_fctn(ExtendedMotionController* emCtrlr, Vector dir, unsigned char speed, ExtendedMotionController::Direction turndir, GyroSensor* gyro) {
    std::cout << "Turnto(dir(" << dir.getX() << ", " << dir.getY() << "), speed(" << speed << "), turndir(" << turndir << "))" << std::endl;

    double angle = 0;
    int tmp_speed = 0;

    // truns to the specified angle
    while(true) {
        try {
            angle = gyro->getVector().getAngle(dir, true, false);
            //tmp_speed = speed;
            tmp_speed = 5;

            //if (turndir == ExtendedMotionController::automatic) std::cout << "direction: " << turndir;

            if (turndir == ExtendedMotionController::automatic && angle < 0) tmp_speed *= -1;  // auto -> anticlockwise
            else if (turndir == ExtendedMotionController::anticlockwise) {
                tmp_speed *= -1;
            }

            if (angle < 0 && turndir == ExtendedMotionController::anticlockwise) turndir = ExtendedMotionController::automatic;
            else if (angle >= 0 && turndir == ExtendedMotionController::clockwise) turndir = ExtendedMotionController::automatic;

            //tmp_speed = tmp_speed * (-0.821 + 0.360 * log(abs(angle)+9.8));  // set speed dependent to missing angle
            /*tmp_speed = tmp_speed * (angle/180);   // set speed dependent to missing angle
            if (tmp_speed < 0) tmp_speed -= 5;
            else tmp_speed += 5;*/

            //tmp_speed = 5;  /// TESTING !!!

            if (abs(angle) > 25) {
                emCtrlr->drive(Vector(), 255);
                boost::this_thread::sleep_for(boost::chrono::milliseconds(10));
                emCtrlr->idle();
                boost::this_thread::sleep_for(boost::chrono::milliseconds(40));
            }
            else {
                emCtrlr->drive(Vector(), -45);
                boost::this_thread::sleep_for(boost::chrono::milliseconds(40));
                emCtrlr->idle();
                break;
            }


            std::cout << angle << "|" << gyro->getVector().getAngle(true, false) << "|" << dir.getAngle(true, false) << " | " << tmp_speed << " | " << abs(angle) << " | " << (int)speed << std::endl;

            boost::this_thread::sleep_for(boost::chrono::milliseconds(8));
        }
        catch (boost::thread_interrupted&) {
            return;
        }
    }
    return;
 }

 void MotionControl::thTurn_fctn(ExtendedMotionController* emCtrlr, Vector dir, unsigned char speed, ExtendedMotionController::Direction turndir, GyroSensor* gyro) {
    // truns to the specified angle relative to robot

    Vector absolute;
    absolute = gyro->getVector() + dir;

    while(true) {
        try {
            if (turndir == ExtendedMotionController::automatic && gyro->getVector().getAngle(absolute, true, false) < 0) speed *= -1;  // auto -> anticlockwise
            else if (turndir == ExtendedMotionController::anticlockwise) speed *= -1;

            if (gyro->getVector().getAngle(absolute, true, false) < 0 && turndir == ExtendedMotionController::anticlockwise) turndir = ExtendedMotionController::automatic;
            else if (gyro->getVector().getAngle(absolute, true, false) >= 0 && turndir == ExtendedMotionController::clockwise) turndir = ExtendedMotionController::automatic;

            emCtrlr->drive(Vector(), speed);

            boost::this_thread::sleep_for(boost::chrono::milliseconds(8));
        }
        catch (boost::thread_interrupted&) {
            return;
        }
    }
 }

 void MotionControl::thPBreak_fctn(ExtendedMotionController* emCtrlr) {
    std::cout << "pbreak()" << std::endl;
    emCtrlr->pbreak();
    boost::this_thread::sleep_for(boost::chrono::milliseconds(30));
    emCtrlr->idle();

    /*while (true) {
        try {
            emCtrlr->pbreak();
            boost::this_thread::sleep_for(boost::chrono::milliseconds(5));
        }
        catch (boost::thread_interrupted&) {
            return;
        }
    }*/
 }

 void MotionControl::thIdle_fctn(ExtendedMotionController* emCtrlr) {

 }

 void MotionControl::thTest_fctn(ExtendedMotionController* emCtrlr) {

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


