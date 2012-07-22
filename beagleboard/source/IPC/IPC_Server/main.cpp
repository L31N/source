
/// SERVER ///

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <string>
#include <iostream>
#include <fstream>

#include "libipc.h"

#define BUF 1024
#define UDS_FILE "/tmp/sock.uds"

using namespace std;

const string SOCKET_LIST = "../../../etc/socket_list.conf";
const string PID_LIST = "../../../etc/pid_list.conf";

int main (int argc, char** argv) {
    if (argc > 1) {     /// argument wurde uebergebenja
        if (string(argv[1]) == "--help") {
            cout << "\nUsage: " << argv[0] << " [SOCKET_FILE]\n" << endl;
            return 0;
        }
        else {
            ifstream is (argv[1], ifstream::in);
            if (!is.is_open()) {
                cout << "Error: cannot open SOCKET_FILE ... " << endl;
                return -1;
            }
        }
    }
    else {
        ifstream is (SOCKET_LIST.c_str(), ifstream::in);
        if (!is.is_open()) {
            cout << "Error: file \"" << SOCKET_LIST << "\" does not exist ..." << endl;
            return -1;
        }
    }

    /// read socket_file from SOCKET_LIST

    cout << "pid: " << get_pid_by_pname("#", PID_LIST.c_str()) << endl;


    return 0;
}

/*int main(int argc, char** argv) {
    int create_socket, new_socket;
    socklen_t addrlen;

    char* buffer = new char[BUF];

    ssize_t size;
    struct sockaddr_un address;

    //const int y = 1;

    if((create_socket=socket (AF_UNIX, SOCK_STREAM, 0)) > 0)
    cout << "Socket wurde angelegt" << endl;
    unlink(UDS_FILE);
    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, UDS_FILE);
    if (bind ( create_socket, (struct sockaddr *) &address, sizeof (address)) != 0) {
        cout << "Der Port ist nicht frei – belegt!" << endl;
    }

    listen (create_socket, 5);

    addrlen = sizeof (struct sockaddr_in);
    while (1) {
        new_socket = accept ( create_socket, (struct sockaddr *) &address, &addrlen );
        if (new_socket > 0) cout << "Ein Client ist verbunden ..." << endl;
        else perror("cant accept the connection from the client");
        do {
            cout << "Nachricht zum Versenden: ";
            fgets (buffer, BUF, stdin);
            send (new_socket, buffer, strlen (buffer), 0);
            size = recv (new_socket, buffer, BUF-1, 0);
            if( size > 0) buffer[size] = '\0';
            cout << "Nachricht empfangen: " << buffer << endl;
        } while (strcmp (buffer, "quit\n") != 0);
     close (new_socket);
  }
  close (create_socket);
  unlink(UDS_FILE);
  return EXIT_SUCCESS;
}
*/
