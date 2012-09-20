
#include <iostream>
#include <string>

#include "bbipc.h"

#include <stdlib.h>

using namespace std;

//const string UDS_FILE_PATH = "/tmp/ipcserver_module.uds";

int main () {

    ipcConfig ipcconfig("../../../../../etc/ipc.conf");

    ipcReceivingConnection* receivingConnection = new ipcReceivingConnection(ipcconfig.getUDS_FILE_PATH(), ipcconfig.getIpcIDToProcessSyn("SENSOR_DATA_MODULE"));
    ipcSendingConnection* sendingConnection1 = new ipcSendingConnection(ipcconfig.getUDS_FILE_PATH(), ipcconfig.getIpcIDToProcessSyn("TACTIC_MODULE"), ipcconfig.getIpcIDToProcessSyn("SENSOR_DATA_MODULE"));
    ipcSendingConnection* sendingConnection2 = new ipcSendingConnection(ipcconfig.getUDS_FILE_PATH(), ipcconfig.getIpcIDToProcessSyn("TCP_MODULE"), ipcconfig.getIpcIDToProcessSyn("SENSOR_DATA_MODULE"));

    sendingConnection1->sendData("datafromsendingconnection1");
    sendingConnection2->sendData("fromSC2");

    sleep(1);

    Data* data = receivingConnection->readDataFromBuffer();
    while(data != 0) {
        cout << data->getData() << "  from:  " << data->getSenderID() << endl;
        data = receivingConnection->readDataFromBuffer();
    }

    delete receivingConnection;
    delete sendingConnection1;
    delete sendingConnection2;

    return 0;
}
