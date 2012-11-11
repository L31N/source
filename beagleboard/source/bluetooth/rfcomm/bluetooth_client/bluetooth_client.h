// bluetooth_client.h

#ifndef _BLUETOOTH_CLIENT_H_
#define _BLUETOOTH_CLIENT_H_

#include "bbbluetooth.h"
#include "ipc/ipc_connection.h"

#include <string>
#include <fstream>

const std::string BLUETOOTH_CONFIG_FILE_PATH = "../../../../../etc/bluetooth.conf";


class BluetoothClient {
    public:
        BluetoothClient();
        ~BluetoothClient();

    private:
        std::string readDestinationFromCfg(const std::string BLUETOOTH_CONFIG_FILE_PATH);

        BluetoothClientSocket* bt_csock;
        ipcReceivingConnection* ipc_rcon;

        void loop(void);
};

#endif // _BLUETOOTH_CLIENT_H_
