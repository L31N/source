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
        std::cout << "waiting to accept" << std::endl;
        bt_ssock->bt_accept();
        std::cout << "accepted connection" << std::endl;
        str = bt_ssock->bt_recv();
        std::cout << "received data" << std::endl;

        /// extract the information from string
        short senderID = str[0];
        short endpointID = str[1];
        std::string data = str.substr(2, std::string::npos);
        data.insert(0,1,senderID);

        /// pipe the data to the correct endpoint
        std::cout << "read BLUETOOTH_MODULE_ID" << std::endl;
        ipcConfig ipcconfig(IPC_CONFIG_FILE_PATH);
        short ipc_bt_id = ipcconfig.getIpcIDToProcessSyn("BLUETOOTH_MODULE");

        std::cout << "BLUETOOTH_MODULE_ID: " << ipc_bt_id << std::endl;

        ipcSendingConnection ipc_scon(ipc_bt_id, endpointID, IPC_LOCAL);
        ipc_scon.sendData(data);

        std::cout << "close client now ..." << std::endl;
        bt_ssock->bt_close_client();

        usleep(2000);
    }
}
