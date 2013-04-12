
// bbcan.cpp

#include "bbcan.h"

const std::string CAN_CONFIG_FILE_PATH = "/bbusr/etc/can.conf";

CAN::CAN(std::string _canName, unsigned char _bufferSize) {
    bufferSize = _bufferSize;
    cancnf = new CANConfig(CAN_CONFIG_FILE_PATH);
    canID = cancnf->getCanID(_canName);

    rcon = new ipcReceivingConnection(cancnf->getIpcSynonym(canID), bufferSize, 16);
    if (!rcon->is_open()) {
        std::cerr << "could not open ipcReceivingConnection" << std::endl;
    }
}

CAN::CAN(unsigned short _canID, unsigned char _bufferSize) {
    bufferSize = _bufferSize;
    cancnf = new CANConfig(CAN_CONFIG_FILE_PATH);
    canID = _canID;

    rcon = new ipcReceivingConnection(cancnf->getIpcSynonym(canID), bufferSize, 16);
    if (!rcon->is_open()) {
        std::cerr << "could not open ipcReceivingConnection" << std::endl;
    }
}

CAN::~CAN() {
    delete cancnf;
    delete rcon;
}

bool CAN::read(CAN::can_t& msg) {
    if (rcon->checkForNewData()) {
        std::string data = rcon->readDataFromBuffer()->getData();
        msg.rtr = data[0];
        msg.length = data[1];
        for (int i = 0; i < msg.length; i++) {
            msg.data[i] = data[2+i];
        }
        return true;
    }
    else return false;
}

bool CAN::write(CAN::can_t& msg) {
    ipcSendingConnection scon(cancnf->getIpcSynonym(canID), "SPI_MSERVER", 16, ipcSendingConnection::local);
    if (!scon.is_open()) {
        std::cerr << "could not open ipcSendingConnection" << std::endl;
        return false;
    }

    std::string data;
    data[0] = canID;
    data[1] = msg.rtr;
    data[2] = msg.length;

    for (unsigned int i = 0; i < msg.length; i ++) data[3+i] = msg.data[i];

    return scon.sendData(data);
}

bool CAN::checkNew(void) {
    return (rcon->checkForNewData());
}
