
#include <iostream>
#include <string>

#include "ipc_config.h"
#include "ipc_connection.h"

#include <stdlib.h>

using namespace std;

//const string UDS_FILE_PATH = "/tmp/ipcserver_module.uds";

int main () {



    //ipcConfig ipcconfig("../../../../../etc/ipc.conf");

    ipcReceivingConnection rc("DEBUG");
    ipcReceivingConnection btrc("BLUETOOTH_MODULE");

    ipcSendingConnection sc ("TACTIC_MODULE", "DEBUG", IPC_BLUETOOTH);

    sc.sendData("teststring");

    Data* data = rc.readDataFromBuffer();
    cout << "data: " << data->getData() << endl;


/*
    ipcReceivingConnection* receivingConnection = new ipcReceivingConnection(ipcconfig.getUDS_FILE_PATH(), ipcconfig.getIpcIDToProcessSyn("TACTIC_MODULE"), 50);
    ipcSendingConnection* sendingConnection = new ipcSendingConnection(ipcconfig.getUDS_FILE_PATH(), ipcconfig.getIpcIDToProcessSyn("SENSOR_DATA_MODULE"), ipcconfig.getIpcIDToProcessSyn("TACTIC_MODULE"));
    sendingConnection->sendData("before, killing receiving");
    delete receivingConnection;

    receivingConnection = new ipcReceivingConnection(ipcconfig.getUDS_FILE_PATH(), ipcconfig.getIpcIDToProcessSyn("TACTIC_MODULE"), 50);
    sendingConnection->sendData("for the new receiving connection ..");

    /// read out buffer
    Data* data = receivingConnection->readDataFromBuffer();
    cout << "data: " << data->getData() << endl;

    delete receivingConnection;

    delete sendingConnection;


    cout << "end is here ............" << endl;*/


    return 0;
}
