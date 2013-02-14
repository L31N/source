
// bbnavi.cpp

#include "bbnavi.h"

Navi::Navi() {

    debug = new Debug("NAVI_API");

    scon = new ipcSendingConnection("NAVI_API", "NAVIGATION_CONTROLLER", 3*sizeof(Vector), ipcSendingConnection::local);
    rcon = new ipcReceivingConnection("NAVI_API", 1, 3*sizeof(Vector));

    if (!scon->is_open()) {
        std::cerr << "ERROR: Navi::Navi() -> could not open ipcSendingConnection ..." << std::endl;
        debug->send("ERROR: Navi::Navi() -> could not open ipcSendingConnection ...");
    }
    if (!rcon->is_open()) {
        std::cerr << "ERROR: Navi::Navi() -> could not open ipcReceivingConnection ..." << std::endl;
        debug->send("ERROR: Navi::Navi() -> could not open ipcReceivingConnection ...");
    }
}

Navi::~Navi() {
    delete scon;
    delete rcon;

    delete debug;
}

void Navi::requestData() {
        rcon->clearBuffer();
        scon->sendData(std::string(0));

        for (int i = 0; !rcon->checkForNewData() && i < 500; i++) usleep(1000);

        if (rcon->checkForNewData()) {
            Data* data = rcon->readDataFromBuffer();

            std::string str_data = data->getData();

            std::string str_position;
            std::string str_direction;
            std::string str_speed;

            str_position = str_data.substr(0, sizeof(Vector));
            str_direction = str_data.substr(sizeof(Vector), sizeof(Vector));
            str_speed = str_data.substr(sizeof(Vector)*2, sizeof(Vector));

            position = Vector(str_position);
            direction = Vector(str_direction);
            speed = Vector(str_speed);
        }
        else {
            std::cerr << "ERROR: Navi::requestData() -> could not request data form NavigationController  ..." << std::endl;
            debug->send("ERROR: Navi::requestData() -> could not request data form NavigationController  ...");
        }

}

Vector Navi::getPosition() {
    return position;
}

Vector Navi::getDirection() {
    return direction;
}

Vector Navi::getSpeed() {
    return speed;
}
