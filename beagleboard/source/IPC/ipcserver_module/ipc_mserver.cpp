
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#include <sys/select.h>

#include <iostream>
#include <string>
#include <cstring>
#include <sstream>

#include <stdio.h>

using namespace std;

int main () {


    const int max_connections = FD_SETSIZE;                         /// describes the max. number of connections who are possible -- for system max. possible use "FD_SETSIZE"
    const string SOCKET_FILE = "/tmp/ipcserver_module.uds";         /// defines the unix-domain-socket-file used for socket-communication

    const short callback_error_by_setting_up_connection = 0;        /// callback which is sent for an error while setting up a new connection
    const short callback_connection_setup_seccessful = 1;           /// callback which is sent after a successful setup of the connection
    //const short callback_endpoint_not_longer_available = 2;         /// callback which is sent when an endpoint of the connection is not longer available
    const short callback_data_delivered_successfully = 3;           /// callback which is sent when the sent data-package is delivered successfully to the endpoint
    const short callback_error_while_sending_data_package = 4;      /// callback which is sent for an error while sending a data-package to an endpoint
    const short callback_wrong_data_format_for_identification = 5;  /// callback which is sent for receiving a wrong data format for the identification-package
    const short callback_receiving_connection_already_exists = 6;   /// callback which is sent when a client try to setup a receiving-connection which already exists


    int client_socks[max_connections];              /// sockets used for the client communication
    short client_IDs[max_connections];              /// contains the client-IDs, the index is related to the socket-descriptors used by the client
    short endpoint_IDs[max_connections];            /// contains the ID of the client-destination, the index is related to the socket-descriptors used by the client

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

    for (i = 0; i < max_connections; i++) {
        client_socks[i] = -1;                       /// set all client_socks[] to -1, -1 is the value for an empty socket which contains no real socket-descriptor
        client_IDs[i] = -1;                         /// set all clientIDs[] to -1, -1 is the value for an empty ID which is not bound to an client
        endpoint_IDs[i] = -1;                       /// set all endpointIDs[] to -1, -1 is the value for an empty ID which is not bound to an endpoint
    }

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
        bool released_connections = false;

        /// check weather new data or new connections were recived
        *readable_sockets = *all_sockets;

        ready = select(max_sock + 1, readable_sockets, NULL, NULL, NULL);
        if (ready == -1) {
            perror("function select()");
            return -1;
        }

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
                bzero (buf, sizeof(buf));
                if ((rval = read(com_sock, buf, 1024)) < 0) { perror("reading stream message"); cout << "com_sock: " << com_sock << endl; }
                else if (rval == 0) {
                    close(com_sock);
                    FD_CLR(com_sock, all_sockets);
                    client_socks[i] = -1;

                    /// reset ID-relations for this client / socket-descriptor
                        /// if it was a receiving connection --> all connections with endpoint to it have to be closed

                    cout << "client closed communication" << endl;

                    /*if (client_IDs[com_sock] == endpoint_IDs[com_sock]) { /// connection was a receiving-connection
                        for (int j = 0; j < max_connections; j++) {
                            //cout << "client_socks[" << j << "]: " << client_socks[j] << " client_IDs[" << j << "]: " << client_IDs[j] << " endpoint_IDs[" << j << "]: " << endpoint_IDs[j] << endl;
                            if (endpoint_IDs[j] == client_IDs[com_sock] && client_IDs[j] != client_IDs[com_sock]) {  /// connections found with endpoint to closed receiving connection but not the receiving connection self
                                /// write back a callback to client which have to close communication
                                    /// find socket-descriptor which contains the connection which should be closed
                                for (int k = 0; k < max_sock + 1; k++) {
                                    if (client_socks[k] == j) {     /// socket-descriptor which contains the right connection found
                                        /// i do not wirte a ending callback, anymore, because this caused crashes if socket is not longer available ...
                                        //if (write(client_socks[k], (char*)&callback_endpoint_not_longer_available, 1) < 0) {
                                            perror("could not send callback to client --> function write()");
                                        }
                                        /// release IDs for this connection
                                        //cout << "delte dependend connection: " << endl;
                                        //cout << "\tclient_IDs: " << client_IDs[client_socks[k]] << endl;
                                        //cout << "\t endpoint_IDs: "  << endpoint_IDs[client_socks[k]] << endl;
                                        //cout << "\t client_socks: " << client_socks[k] << endl;

                                        client_IDs[client_socks[k]] = -1;
                                        endpoint_IDs[client_socks[k]] = -1;

                                        close(client_socks[k]);
                                        FD_CLR(client_socks[k], all_sockets);

                                        released_connections = true;
                                    }
                                    //cout << "sent callback (endpoint_not_longer_available) to client: " << client_socks[k] << endl;
                                }
                                cout << "closed connection to released endpoint" << endl;
                            }
                        }
                    }*/
                    /// release IDs for the connections from closed receiving connection
                    client_IDs[com_sock] = -1;
                    endpoint_IDs[com_sock] = -1;
                }
                else {      /// recived data from client
                    if (client_IDs[com_sock] == -1) {   /// read identification and save it to client_IDs[]
                        if (strlen(buf) == 2) {

                            /// check for an receiving connection

                            /// conversion to short
                            short byte0 = buf[0];
                            short byte1 = buf[1];
                            /*stringstream ss_0;
                            stringstream ss_1;
                            ss_0 << buf[0];
                            ss_0 >> byte0;
                            ss_1 << buf[1];
                            ss_1 >> byte1;*/

                            if (byte0 == byte1) {   /// its a receiving connection
                                bool con_exists = false;
                                for (int j = 0; j < max_connections; j++) {   /// check weather the receiving-connection already exists
                                    if (byte0 == client_IDs[j] || byte1 == endpoint_IDs[j]) {   /// the receiving-connections already exists
                                        con_exists = true;
                                        if (write(com_sock, (char*)&callback_receiving_connection_already_exists, 1) < 0) {
                                            perror("could not send callback to client --> function write()");
                                        }
                                        cout << "receiving connection already exists ..." << endl;
                                        break;
                                    }

                                }
                                if (!con_exists) {      /// receiving-connections do not exist, so set it here
                                    client_IDs[com_sock] = byte0;
                                    endpoint_IDs[com_sock] = byte1;
                                    cout << "new receiving connection set ... [" << client_IDs[com_sock] << "] [" << endpoint_IDs[com_sock] << "]" << endl;
                                }
                                else continue;
                            }
                            else {  /// check weather the endpoint is already known
                                for (int j = 0; j < max_connections; j++) {
                                    if (client_IDs[j] == byte1) {   /// endpoint already known -> setup IDs
                                        client_IDs[com_sock] = byte0;
                                        endpoint_IDs[com_sock] = byte1;
                                    }
                                }
                            }
                            /// send error message back, if endpoint is not already known (callback 0)
                            if (client_IDs[com_sock] == -1 || client_IDs[com_sock] == -1) {
                                if (write(com_sock, (char*)&callback_error_by_setting_up_connection, 1) < 0) {
                                    perror("could not send callback to client --> function write()");
                                }
                                cout << "received unknown endpoint ..." << endl;
                            }
                            else {  /// setting up connection was successful (callback 1)
                                if (write(com_sock, (char*)&callback_connection_setup_seccessful, 1) < 0) {
                                    perror("could not send callback to client --> function write()");
                                }
                                //cout << "connection was successfully set ..." << endl;
                            }
                        }
                        else {  /// wrong data format
                            if (write(com_sock, (char*)&callback_wrong_data_format_for_identification, 1) < 0) {
                                perror("could not send callback to client --> function write()");
                            }

                            cout << "error: received wrong data format, expected ID-package (2byte)" << endl;
                            continue;
                        }
                    }
                    else {  /// work with received data
                        /*stringstream ss_client_incom;
                        stringstream ss_endpoint_incom;
                        char cclient;
                        char cendpoint;
                        ss_client_incom << client_IDs[com_sock];
                        ss_client_incom >> cclient;
                        ss_endpoint_incom << endpoint_IDs[com_sock];
                        ss_endpoint_incom >> cendpoint;*/
                        cout << "received data from sender: " << client_IDs[com_sock] << " with endpoint: " << endpoint_IDs[com_sock] << endl;

                        /// create new string with right formatting to send it to the endpoint
                        string data_for_endpoint = buf;
                        /*stringstream ss_endpoint;
                        char csender;
                        ss_endpoint << client_IDs[com_sock];
                        ss_endpoint >> csender;*/
                        data_for_endpoint.insert(0, 1, client_IDs[com_sock]);
                        cout << "the output string will look like this: " << data_for_endpoint << "  |" << endl;

                        int endpoint_sock = -1;
                        /// find the right socket-descriptor for endpoint
                        for (int j = 0; j < max_connections; j++) {
                            if (client_IDs[j] == endpoint_IDs[com_sock]) {
                                for (int k = 0; k < max_sock + 1; k++) {
                                    if (client_socks[k] == j) {
                                        endpoint_sock = client_socks[k];
                                        break;
                                    }
                                }
                                break;
                            }
                        }

                        //cout << "endpoint filedescriptor: " << endpoint_sock << endl;

                        if (endpoint_sock != -1) {      /// the socket-descriptor for the right endpoint was found
                            if (write(endpoint_sock, data_for_endpoint.c_str(), data_for_endpoint.length()) < 0) {
                                perror("could not send data-package to endpoint --> function write()");
                                if (write(com_sock, (char*)&callback_error_while_sending_data_package, 1) < 0) {
                                    perror("could not send callback to client --> function write()");
                                }
                            }
                            else {
                                if (write(com_sock, (char*)&callback_data_delivered_successfully, 1) < 0) {
                                    perror("could not send callback to client --> function write()");
                                }
                            }
                        }
                        else {      /// could not find socket-descriptor for endpoint
                            if (write(com_sock, (char*)&callback_error_while_sending_data_package, 1) < 0) {
                                perror("could not send callback to client --> function write()");
                            }
                        }
                    }
                }

                /// check for more ready descriptors
                if (--ready <= 0) break;    //seems not so
            }
            if (released_connections) break;    /// break and start from beginning to load the sock-lists again ...
        }
    } /// while(true)

    cout << "after while(true) --> strange !!!" << endl;


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
