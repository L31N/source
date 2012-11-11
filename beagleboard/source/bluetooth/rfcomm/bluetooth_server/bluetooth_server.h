// bluetooth_server.h

#ifndef _BLUETOOTH_SERVER_H_
#define _BLUETOOTH_SERVER_H_

#include "ipc/ipc_connection.h"
#include "bbbluetooth.h"

#include <string>


class BluetoothServer {
    public:
        BluetoothServer();
        ~BluetoothServer();



    private:
        BluetoothServerSocket* bt_ssock;

};

#endif // _BLUETOOTH_SERVER_H_
