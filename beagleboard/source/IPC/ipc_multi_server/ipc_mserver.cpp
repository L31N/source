
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#include <sys/select.h>

#include <iostream>
#include <string>

#include <stdio.h>

using namespace std;

const int max_connections = FD_SETSIZE;             /// describes the max. number of connections who are possible -- for system max. possible use "FD_SETSIZE"

const string SOCKET_FILE = "/tmp/ipc_testing/ipc_mserver.uds";

int main () {
    int client_socks[max_connections];              /// sockets used for the client communication
    int max_sock = -1;                              /// the highest used socket-descriptor used as parameter for function select()
    int max = -1;                                   /// the highest used index of "client_socks" used to check all sockets up to this index-value

    int server_sock,                                /// serversock is used to recive new connections,
        msgsock,                                    /// msgsock recives the new socket-descriptor from function accept()
        com_sock;                                   /// com_sock is used for the real data-transfer with the clients

    int rval = 0;                                   /// used to check weather the client is sending data or has ended
    int ready = 0;                                  /// contains the return value of function select() --> how many socket-descriptors are ready to communicate

    int i = 0;                                      /// used for the for-loops and must be global to check it later for the higest value it has contained in the loop

    struct sockaddr_un addr;                        /// the sockaddr (unix-type) used to identify and bind the socket to an specific unix-domain-file
    char buf[1024];                                 /// the buffer which contains the data recived by the client

    fd_set* readable_sockets = new fd_set;          /// the fd_set contains the amount of all used readable socket-descriptors
    fd_set* writeable_sockets = new fd_set;         /// the fd_set contains the amount of all used writeable socket-descriptors
    fd_set* all_sockets = new fd_set;               /// the fd_set contains the whole amount of all used socket-descriptors

    FD_ZERO(readable_sockets);                      /// sets the fd_set of the readable socket-descriptors to ZERO
    FD_ZERO(writeable_sockets);                     /// sets the fd_set of the writeable socket-descriptors to ZERO
    FD_ZERO(all_sockets);                           /// sets the fd_set of all socket-descriptors to ZERO

    for (i = 0; i < max_connections; i++)           /// set all client_socks to -1, -1 is the value for an empty socket which contains no real socket-descriptor
        client_socks[i] = -1;

    /// setup of server_sock ///
    unlink(SOCKET_FILE.c_str());

    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_FILE.c_str());

    server_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("function socket()");
        return -1;
    }

    if (bind(server_sock, (sockaddr*)&addr, sizeof(sockaddr_un)) == -1) {
        perror("function bind()");
        return -1;
    }

    if (listen(server_sock, 5) == -1) {
        perror("function listen()");
        return -1;
    }

    cout << "server is active now ...\nuds-file --> " << addr.sun_path << "\nwaiting for connections now\n\n" << endl;

    max_sock = server_sock;
    FD_SET(server_sock, all_sockets);

    /// the main-loop
    while(true) {
        /// check weather new data or new connections were recived
        readable_sockets = all_sockets;
        ready = select(max_sock + 1, readable_sockets, NULL, NULL, NULL);

        /// check for a new connection
        if (FD_ISSET(server_sock, readable_sockets)) {
            msgsock = accept(server_sock, 0, 0);
            if (msgsock < 0) {
                perror("function accept()");
                return -1;
            }
            else cout << "connecton established" << endl;

            /// search for a new free place for msgsock in client_socks[]
            for (i = 0; i < max_connections; i++) {
                if (client_socks[i] == -1) {
                    client_socks[i] = msgsock;
                    break;
                }
            }

            if (i == max_connections) {
                cout << "too many clients try to connect... max number of clients are: " << max_connections << endl;
                return -1;
            }

            /// Add new client-socket-descriptor to the amount of readable sockets
            FD_SET(msgsock, all_sockets);

            /// test for the highest socket-descriptor --> needed as parameter for function select()
            if (msgsock > max_sock) max_sock = msgsock;

            /// find the highest index of client_socks[]
            if (i > max) max = i;

            /// check for more ready descriptors
            if (--ready <= 0) continue;  // its seems not so
        }

        /// check here all clients for the arrival of new data
        for (i = 0; i <= max; i++) {
            com_sock = client_socks[i];
            if (com_sock == -1) continue;

            if (FD_ISSET(com_sock, readable_sockets)) {
                /// read the data from client
                do {
                    bzero (buf, sizeof(buf));
                    if ((rval = read(msgsock, buf, 1024)) < 0) perror("reading stream mesasge");
                    else if (rval == 0) cout << "ending connection" << endl;
                    else cout << "recived message: " << buf << endl;
                } while (rval > 0);

                string str = buf;
                if (str == "quit") {
                    close(com_sock);
                    FD_CLR(com_sock, all_sockets);
                    client_socks[i] = -1;
                    cout << "client closed communication" << endl;
                }

                /// check for more ready descriptors
                if (--ready <= 0) break;    //seems not so
            }
        }
    } /// while(true)


    for (i = 0; i < max; i++) {
        if (client_socks[i] >= 0) {
            close(client_socks[i]);
            client_socks[i] = -1;
        }
    }

    close(msgsock);
    close(server_sock);

    unlink(SOCKET_FILE.c_str());

    delete readable_sockets;
    delete writeable_sockets;

    delete all_sockets;

    return 0;

}

    /*    addresses[i].sun_family = AF_UNIX;
        strcpy(addresses[i].sun_path, SOCKET_FILES[i].c_str());

        unlink(SOCKET_FILES[i].c_str());

        sockets[i] = socket(AF_UNIX, SOCK_STREAM, 0);
        if (sockets[i] < 0) {
            perror ("function socket()");
            return -1;
        }

        if (bind(sockets[i], (struct sockaddr*)&addresses[i], sizeof(struct sockaddr_un)) < 0) {
            perror("function bind()");
            return -1;
        }

        if (listen(sockets[i], 1) == -1) {
            perror("function listen(): ");
            return -1;
        }

        cout << "socket_path -->" << addresses[i].sun_path << "\t sock_num: " << sockets[i] << endl;

        FD_SET(sockets[i], &readable_sockets);
        if (sockets[i] > max_sock) max_sock = sockets[i];
    }

    while(true) {
        active_socket = select(max_sock + 1, &readable_sockets, NULL, NULL, NULL);
        cout << "max_sock: " << max_sock << endl;
        cout << "active_socket: " << active_socket << endl;
        if (active_socket < 0) {
            perror("function select() returned trash");
            continue;
        }

        //if (FD_ISSET(msgsock, &readable_sockets)) {
            msgsock = accept(active_socket, 0, 0);

            if (msgsock == -1) perror("function accept()");
            else do {
                bzero (buf, sizeof(buf));
                if ((rval = read(msgsock, buf, 1024)) < 0) perror("reading stream mesasge");
                else if (rval == 0) cout << "ending connection" << endl;
                else cout << "recived message: " << buf << endl;
            } while (rval > 0);

            close(msgsock);
        //}

        int tmp = 0;
        cin >> tmp;

    }

    for (int i = 0; i < max_connections; i++) {
        close(sockets[i]);
        unlink(SOCKET_FILES[i].c_str());
    }

    FD_ZERO(&readable_sockets);
    FD_ZERO(&writeable_sockets);

    return 0;

}
*/
