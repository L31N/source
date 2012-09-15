
#include <iostream>
#include <string>

#include "buffer.h"
#include "ipc_connection.h"

#include <stdlib.h>

using namespace std;

const string UDS_FILE_PATH = "/tmp/ipcserver_module.uds";

int main () {
    ipcReceivingConnection* receivingConnection = new ipcReceivingConnection(UDS_FILE_PATH, 49);
    ipcSendingConnection* sendingConnection1 = new ipcSendingConnection(UDS_FILE_PATH, 50, 49);
    ipcSendingConnection* sendingConnection2 = new ipcSendingConnection(UDS_FILE_PATH, 51, 49);

    sendingConnection1->sendData("data_from_sending_connection_1");
    sendingConnection2->sendData("data_from_sending_connection_2");

    Data* data = receivingConnection->readDataFromBuffer();

    cout << "data: " << data->getData() << "\t| senderID: " << data->getSenderID() << endl;

    delete receivingConnection;
    delete sendingConnection1;
    delete sendingConnection2;

    return 0;
}
