
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
    for (unsigned int i = datastr.length(); i < 34; i++) datastr += char(0);
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
    std::string datastr = "";
    datastr += char(2);

    char cVector[sizeof(Vector)];
    memcpy(cVector, &vector, sizeof(Vector));
    datastr.append(cVector, sizeof(cVector));

    datastr += char(speed);

    char cDir[sizeof(Vector)];
    memcpy(cDir, &dir, sizeof(Vector));
    datastr.append(cDir, sizeof(cDir));
    for (unsigned int i = datastr.length(); i < 34; i++) datastr += char(0);
    scon->sendData(datastr);

    return;
}

void Motion::move(Vector vector, unsigned char speed, Angle dir) {
    Vector direction(0,1);
    direction.setAngle(dir.value());
    this->move(vector, speed, direction);
}

void Motion::turnto(Vector dir, unsigned char speed, Motion::Direction turndir) {
    std::string datastr = "";
    datastr += char(3);

    char cVector[sizeof(Vector)];
    memcpy(cVector, &dir, sizeof(Vector));
    datastr.append(cVector, sizeof(cVector));

    datastr += char(speed);
    datastr += char(turndir);
    for (unsigned int i = datastr.length(); i < 34; i++) datastr += char(0);
    scon->sendData(datastr);

    return;
}

void Motion::turnto(Angle dir, unsigned char speed, Motion::Direction turndir) {
    Vector direction(0,1);
    direction.setAngle(dir.value());

    this->turnto(direction, speed, turndir);
}

void Motion::turn(Vector dir, unsigned char speed, Direction turndir) {
    std::string datastr = "";
    datastr += char(4);

    char cVector[sizeof(Vector)];
    memcpy(cVector, &dir, sizeof(Vector));
    datastr.append(cVector, sizeof(cVector));

    datastr += char(speed);
    datastr += char(turndir);
    for (unsigned int i = datastr.length(); i < 34; i++) datastr += char(0);
    scon->sendData(datastr);

    return;
}

void Motion::turn(Angle dir, unsigned char speed, Direction turndir) {
    Vector direction(0,1);
    direction.setAngle(dir.value());

    this->turn(direction, speed, turndir);
}


void Motion::pbreak() {
    std::string datastr = "";
    datastr += char(5);
    for (unsigned int i = datastr.length(); i < 34; i++) datastr += char(0);
    scon->sendData(datastr);

    return;
}

void Motion::idle() {
    std::string datastr = "";
    datastr += char(6);
    for (unsigned int i = datastr.length(); i < 34; i++) datastr += char(0);
    scon->sendData(datastr);

    return;
}

void Motion::test(unsigned char speed){
    std::string datastr = "";
    datastr += char(7);
    for (unsigned int i = datastr.length(); i < 34; i++) datastr += char(0);
    scon->sendData(datastr);

    return;
}
