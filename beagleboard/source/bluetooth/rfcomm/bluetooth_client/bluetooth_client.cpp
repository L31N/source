// bluetooth_client.cpp

#include "bluetooth_client.h"

/** CLASS BLUETOOTH_CLIENT **/

BluetoothClient::BluetoothClient() {
    std::string dest = readDestinationFromCfg(BLUETOOTH_CONFIG_FILE_PATH);
    if (dest == "") {
        std::cerr << "cannot read destination from config file: " << BLUETOOTH_CONFIG_FILE_PATH << std::endl;
        return;
    }

    bt_csock = new BluetoothClientSocket(dest);
    ipc_rcon = new ipcReceivingConnection("BLUETOOTH_MODULE", 50);

}

BluetoothClient::~BluetoothClient() {
    delete bt_csock;
    delete ipc_rcon;
}

std::string BluetoothClient::readDestinationFromCfg(const std::string BLUETOOTH_CONFIG_FILE_PATH) {
    std::ifstream ifs (BLUETOOTH_CONFIG_FILE_PATH.c_str());

    std::string buf;

    while(!ifs.eof()) {
        std::getline(ifs, buf);
        if (buf[0] != '#' && buf != "") {
            ifs.close();
            return buf;
        }
    }
    ifs.close();
    return "";
}

void BluetoothClient::loop() {
    Data* data = NULL;
    while(true) {
        data = ipc_rcon->readDataFromBuffer();
        while (data != NULL) {     /// new data in buffer
            short senderID = data->getSenderID();
            std::string str = data->getData();

            str.insert(0, 1, senderID);

            bt_csock->bt_connect();
            bt_csock->bt_send(str);

            data = ipc_rcon->readDataFromBuffer();
        }

        usleep(2000);
    }
}
