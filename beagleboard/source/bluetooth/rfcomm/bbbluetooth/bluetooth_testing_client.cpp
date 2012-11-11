
#include "bbbluetooth.h"

#include <string>
#include <iostream>

using namespace std;

int main (int argc, char** argv) {

    if (argc != 3) {
        cout << "usage: " << argv[0] << " DEST_ADDR MESSAGE" << endl;
        return -1;
    }

    BluetoothClientSocket* bt_csock = new BluetoothClientSocket(string(argv[1]));
    bt_csock->bt_connect();
    bt_csock->bt_send(string(argv[2]));

    return 0;
}
