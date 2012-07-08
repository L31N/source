
#include <iostream>

#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>


/// USAGE: BluetoothChatServer <bdaddr> <channel>

using namespace std;

int main(int argc, char **argv) {

    if (argc != 3) {    /// tow few or much arguements
        cout << "usage: BluetoothChatClient <bdaddr> <channel>" << endl;
        return -1;
    }

    struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
    char buf[1024] = { 0 };
    int s, client, bytes_read;
    socklen_t opt = sizeof(rem_addr);

    // allocate socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // bind socket to port 1 of the first available
    // local bluetooth adapter
    loc_addr.rc_family = AF_BLUETOOTH;
    //loc_addr.rc_bdaddr = *BDADDR_ANY;
    loc_addr.rc_bdaddr = *BDADDR_ANY;
    //loc_addr.rc_channel = (uint8_t) argv[3];
    loc_addr.rc_channel = *(unsigned char*) argv[2];

    if (bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr)) == -1) {
        cout << "error in function bind() ..." << endl;
        return -1;
    }

    // put socket into listening mode
    if (listen(s, 1) == -1) {
        cout << "error in function listen() ..." << endl;
        return -1;
    }

    /// MAIN-SERVER-LOOP #####
    while(true) {
        cout << "\nwaiting for connections ..." << endl;

        // accept one connection
        client = accept(s, (struct sockaddr *)&rem_addr, &opt);
        if (client < 0) {
            cout << "error in function accept() ..." << endl;
            return -1;
        }

        // read data from the client
        bytes_read = read(client, buf, sizeof(buf));
        if( bytes_read > 0 ) {
            cout << "recived data: " << buf << endl;
        }
        else cout << "error while reciving data ..." << endl;

        usleep(5000);

        close(client);

    }
    close(s);
    return 0;
}
