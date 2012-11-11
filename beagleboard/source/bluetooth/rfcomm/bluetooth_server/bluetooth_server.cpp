// bluetooth_server.cpp

#include "bluetooth_server.h"

/** CLASS BLUETOOTH_SERVER **/

BluetoothServer::BluetoothServer() {
    bt_ssock = new BluetoothServerSocket();
    bt_ssock->bt_bind();
    bt_ssock->bt_listen();

    bt_ssock->bt
}
