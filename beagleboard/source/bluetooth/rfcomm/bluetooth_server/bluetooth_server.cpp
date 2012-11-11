// bluetooth_server.cpp

#include "bluetooth_server.h"

/** CLASS BLUETOOTH_SERVER **/

BluetoothServer::BluetoothServer() {
    bt_ssock = new BluetoothServerSocket();
    bt_ssock->bt_bind();
    bt_ssock->bt_listen();

}

BluetoothServer::~BluetoothServer() {
    delete bt_ssock;
}

void BluetoothServer::loop() {
    std::string str;

    while(true) {
        bt_ssock->bt_accept();
        str = bt_ssock->bt_recv();

        /// extract the information from string
        short senderID = str[0];
        short endpointID = str[2];
        std::string data = str.substr(3, std::string::npos);
        data.insert(0, 1, senderID);

        /*std::cout << "senderID: " << senderID << std::endl;
        std::cout << "host: " << host << std::endl;
        std::cout << "endpointID: " << endpointID << std::endl;
        std::cout << "data: " << data << std::endl;*/

        /// pipe the data to the correct endpoint
        ipcSendingConnection ipc_scon(senderID, endpointID, IPC_LOCAL);
        ipc_scon.sendData(data);


        bt_ssock->bt_close_client();
    }
}
