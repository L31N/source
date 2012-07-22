
/// CLIENT ///

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

#define BUF 1024
#define UDS_FILE "/tmp/sock.uds"

using namespace std;

int main (int argc, char** argv) {
    int create_socket;
    char *buffer = new char[BUF];
    struct sockaddr_un address;
    int size;
    printf ("\e[2J");
    if((create_socket=socket (AF_UNIX, SOCK_STREAM, 0)) > 0)
    cout << "Socket wurde angelegt" << endl;
    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, UDS_FILE);
    if (connect ( create_socket,
                (struct sockaddr *) &address,
                sizeof (address)) == 0)
    cout << "Verbindung mit dem Server hergestellt" << endl;
    do {
      size = recv(create_socket, buffer, BUF-1, 0);
      if( size > 0)
         buffer[size] = '\0';
      cout << "Nachricht erhalten: " << buffer << endl;
      if (strcmp (buffer, "quit\n")) {
         cout << "Nachricht zum Versenden: ";
         fgets (buffer, BUF, stdin);
         send(create_socket, buffer, strlen (buffer), 0);
       }
    } while (strcmp (buffer, "quit\n") != 0);
    close (create_socket);
    return EXIT_SUCCESS;
}
