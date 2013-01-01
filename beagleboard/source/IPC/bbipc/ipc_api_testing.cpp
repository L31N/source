
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

    cout << "rc_open: " << rc.is_open() << endl;
    cout << "btrc_open: " << btrc.is_open() << endl;

    //ipcSendingConnection sc ("TACTIC_MODULE", "DEBUG", IPC_LOCAL);

    //sc.sendData("teststring");
    //sc.sendData("secondstring");

    //sleep(1);

    //sc.sendData("helloworld");

    /*if (rc.checkForNewData()) {
        Data* data = rc.readDataFromBuffer();
        cout << "data read ..." << endl;
        std::string str = data->getData();
        cout << "data: " << str << endl;
    }
    else cout << "no data available ..." << std::endl;*/


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
