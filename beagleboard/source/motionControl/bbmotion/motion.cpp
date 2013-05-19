
// motion.cpp

#include "motion.h"

Motion::Motion() {
    dbg = new Debug("MOTION_DBG");
    scon = new ipcSendingConnection("MOTION", "MOTION_CONTROL", 34, ipcSendingConnection::local);

    if (!scon->is_open()) {
        std::cerr << "Error could not open ipcSendingConnection ..." << std::endl;
        dbg->send("Error could not open ipcSendingConnection ...");
    }
}

Motion::~Motion() {
    delete dbg;
    delete scon;
}

void Motion::drive(Vector vector, short rotationSpeed) {
    std::string datastr = "";
    datastr += char(0);

    char cVector[sizeof(Vector)];
    memcpy(cVector, &vector, sizeof(vector));
    datastr.append(cVector, sizeof(cVector));

    char cRotSpeed[sizeof(short)];
    memcpy(cRotSpeed, &rotationSpeed, sizeof(short));
    datastr.append(cRotSpeed, sizeof(short));

    for (unsigned int i = datastr.length(); i < 34; i++) datastr += char(0);

    scon->sendData(datastr);

    return;
}

void Motion::drive(Angle angle, unsigned char speed, short rotationSpeed) {
    Vector vector;
    vector.setAngle(angle.value());
    vector.setLength(speed);

    this->drive(vector, rotationSpeed);

    return;
}

void Motion::moveto(Vector vector, unsigned char speed, Vector dir) {
    std::string datastr = "";
    datastr += 1;

    char cVector[sizeof(Vector)];
    memcpy(cVector, &vector, sizeof(Vector));
    datastr.append(cVector, sizeof(cVector));

    datastr += char(speed);

    char cDir[sizeof(Vector)];
    memcpy(cDir, &dir, sizeof(Vector));
    datastr.append(cDir, sizeof(cDir));

    scon->sendData(datastr);

    return;

}

void Motion::moveto(Vector vector, unsigned char speed, Angle dir) {
    Vector dirVect;
    dirVect.setAngle(dir.value());
    dirVect.setLength(speed);

    this->moveto(vector, speed, dirVect);

    return;
}

void Motion::move(Vector vector, unsigned char speed, Vector dir) {

}

void Motion::move(Vector vector, unsigned char speed, Angle dir) {

}

void Motion::turnto(Vector dir, unsigned char speed, Motion::Direction turndir) {

}

void Motion::turnto(Angle dir, unsigned char speed, Motion::Direction turndir) {

}

void Motion::pbreak() {

}

void Motion::idle() {

}
