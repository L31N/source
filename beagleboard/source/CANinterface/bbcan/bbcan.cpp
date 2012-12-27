
#include "bbcan.h"

const std::string CAN_CONFIG_FILE_PATH = "../../../../etc/can.conf";

CAN::CAN(const std::string _MODULE_NAME) {
    MODULE_NAME = _MODULE_NAME;
    debug = new Debug(MODULE_NAME);
    cancfg = new CANConfig(CAN_CONFIG_FILE_PATH);
}

CAN::~CAN() {
    delete debug;
    delete cancfg;
}

void CAN::init_member(std::string can_member) {
    ipcReceivingConnection* tmp_ipcRCon = new ipcReceivingConnection(can_member);
    if (tmp_ipcRCon->is_open()) {
        receivingConnections.push_back(Rcon(tmp_ipcRCon, can_member));
    }
    else {
        std::cerr << "ERROR: could not open receivingConnection to: " << can_member << std::endl;
        debug->send("ERROR: could not open receivingConnection to: %s", can_member.c_str());
    }
}

char* CAN::getValue(std::string can_member) {
    for (unsigned int i = 0; i < receivingConnections.size(); i++) {
        if (receivingConnections[i].getCANMember() == can_member) {
            Data* can_data = receivingConnections[i].getIPCRcon()->readDataFromBuffer();
            std::string data = can_data->getData();
            char* cdata = new char[8];
            for (int i = 0; i < 8; i++) {
                cdata[i] = data[i];
            }
            return cdata;
        }
    }
    std::cerr << "ERROR: " << can_member << " was not initialized !!!" << std::endl;
    debug->send("ERROR: %s was not initialized !!!", can_member.c_str());
    return NULL;
}

void CAN::setValue(std::string can_member, char* value) {

}

void CAN::setFilter(unsigned short mask, unsigned short id) {

}


/** CLASS RCON **/

Rcon::Rcon(ipcReceivingConnection* _ipcRCon, std::string _can_member) {
    ipcRCon = _ipcRCon;
    can_member = _can_member;
}

Rcon::~Rcon() {
    delete ipcRCon;
}

ipcReceivingConnection* Rcon::getIPCRcon() { return ipcRCon; }
std::string Rcon::getCANMember() { return can_member; }

