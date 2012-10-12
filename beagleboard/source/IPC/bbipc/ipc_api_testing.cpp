
#include <iostream>
#include <string>

#include "bbipc.h"

#include <stdlib.h>

using namespace std;

//const string UDS_FILE_PATH = "/tmp/ipcserver_module.uds";

int main () {

    ipcConfig ipcconfig("../../../../../etc/ipc.conf");

    ipcReceivingConnection* receivingConnection = new ipcReceivingConnection(ipcconfig.getUDS_FILE_PATH(), ipcconfig.getIpcIDToProcessSyn("SENSOR_DATA_MODULE"), 50);
    /*ipcSendingConnection* sendingConnection1 = new ipcSendingConnection(ipcconfig.getUDS_FILE_PATH(), ipcconfig.getIpcIDToProcessSyn("TACTIC_MODULE"), ipcconfig.getIpcIDToProcessSyn("SENSOR_DATA_MODULE"));
    ipcSendingConnection* sendingConnection2 = new ipcSendingConnection(ipcconfig.getUDS_FILE_PATH(), ipcconfig.getIpcIDToProcessSyn("TCP_MODULE"), ipcconfig.getIpcIDToProcessSyn("SENSOR_DATA_MODULE"));
*/

    ipcSendingConnection* sendingConnections[50];

    for (int i = 0; i < 50; i++) {
        sendingConnections[i] = new ipcSendingConnection(ipcconfig.getUDS_FILE_PATH(), ipcconfig.getIpcIDToProcessSyn("TACTIC_MODULE"), ipcconfig.getIpcIDToProcessSyn("SENSOR_DATA_MODULE"));
    }

    for (int i = 0; i < 50; i++) {
        std::string sendingData;
        sendingData += char(i);
        sendingConnections[i]->sendData(sendingData);
    }

    sleep(1);

    Data* data = receivingConnection->readDataFromBuffer();
    while(data != 0) {
        cout << data->getData() << "  from:  " << data->getSenderID() << endl;
        data = receivingConnection->readDataFromBuffer();
    }

    delete receivingConnection;
    /*delete sendingConnection1;
    delete sendingConnection2;*/
    for (int i = 0; i < 50; i++) delete sendingConnections[i];

    return 0;
}
