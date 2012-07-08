
#include <iostream>
#include <string>

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>


using namespace std;

int main (int argc, char** argv) {

    if (argc != 3) {
        cout << "usage: BluetoothChatClient <bdaddr> <channel>" << endl;
        return -1;
    }

    struct sockaddr_rc addr = { 0 };
    int s, status;
    char *dest = argv[1];

    // allocate a socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    if (s == -1) {
        cout << "error in function socket() ..." << endl;
        return -1;
    }

    // set the connection parameters (who to connect to)
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = *(unsigned char*) argv[2];
    str2ba( dest, &addr.rc_bdaddr );

    // connect to server
    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));
    if (status == -1) {
        cout << "error in function connect() ..." << endl;
        return -1;
    }

    string input = "";
    while(true) {
        cin >> input;
        // send a message
        if( status == 0 ) {
            status = write(s, input.c_str(), input.size());
            if (status < 0) {
                cout << "error occured while sending data to server ..." << endl;
            }
        }
    }

    close(s);
    return 0;
}
