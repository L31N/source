
#include <iostream>
#include <string>

#include "bbipc.h"

#include <stdlib.h>

using namespace std;

//const string UDS_FILE_PATH = "/tmp/ipcserver_module.uds";

int main () {

    ipcConfig ipcconfig("../../../../../etc/ipc.conf");

    //ipcReceivingConnection* receivingConnection = new ipcReceivingConnection(ipcconfig.getUDS_FILE_PATH(), ipcconfig.getIpcIDToProcessSyn("SENSOR_DATA_MODULE"), 50);
    ipcSendingConnection* sendingConnection = new ipcSendingConnection(ipcconfig.getUDS_FILE_PATH(), ipcconfig.getIpcIDToProcessSyn("SENSOR_DATA_MODULE"), ipcconfig.getIpcIDToProcessSyn("TACTIC_MODULE"));


    cout << "before deletion" << endl;

    delete sendingConnection;


    return 0;
}
