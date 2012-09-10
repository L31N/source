
#include <iostream>
#include <string>

#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

//#include <bluetooth/bluetooth.h>
//#include <bluetooth/rfcomm.h>


using namespace std;

int main (int argc, char** argv) {

    if (argc != 2) {
        cout << "usage: BluetoothChatClient <IP_ADDRESS>" << endl;
        return -1;
    }

    cout << "\nip_addr:\t\t " << argv[1] << "\t\tconnecting ... \n" << endl;
    cout << "##############################################################\n" << endl;

    string input = "";
    while(input != ":q") {

        struct sockaddr_in sock = { 0 };
        int s, status;
        unsigned long dest = inet_addr(argv[1]);

        // allocate a socket
        s = socket(AF_INET, SOCK_STREAM, 0);
        if (s == -1) {
            //cout << "error in function socket() ..." << endl;
            perror("error in function socket()");
            return -1;
        }

        memset (&sock, 0, sizeof (sock));

        // set the connection parameters (who to connect to)
        memcpy ((char*)&sock.sin_addr, &dest, sizeof(dest));
        sock.sin_family = AF_INET;
        sock.sin_port = htons(5321);


        // connect to server
        status = connect(s, (struct sockaddr *)&sock, sizeof(sock));
        if (status == -1) {
            //cout << "error in function connect() ..." << endl;
            perror("error in function connect()");
            return -1;
        }

        cout << "data to send: ";
        getline (cin, input, '\n');
        if (input == ":q") {
            cout << "\nquit now ...\n" << endl;
            return 0;
        }
        // send a message
        if( status == 0 ) {
            status = write(s, input.c_str(), input.size());
            if (status < 0) {
                //cout << "error occured while sending data to server ..." << endl;
                perror("error occured while sending data to server");
                return -1;
            }
        }
        close(s);
    }

    return 0;
}


/*
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

int main(int argc, char **argv)
{
    struct sockaddr_rc addr = { 0 };
    int s, status;
    char dest[18] = "00:1B:DC:05:7E:24";

    // allocate a socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // set the connection parameters (who to connect to)
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba( dest, &addr.rc_bdaddr );

    // connect to server
    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));

    // send a message
    if( status == 0 ) {
        status = write(s, "hello!", 6);
    }

    if( status < 0 ) perror("uh oh");

    close(s);
    return 0;
}
*/
