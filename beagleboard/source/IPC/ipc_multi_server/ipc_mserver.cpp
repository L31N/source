
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#include <sys/select.h>

#include <iostream>
#include <string>

#include <stdio.h>

using namespace std;

const int max_connections = 5;

const string SOCKET_FILES [max_connections] = {
    "/tmp/ipc_testing/ipc_mserver_00.uds",
    "/tmp/ipc_testing/ipc_mserver_01.uds",
    "/tmp/ipc_testing/ipc_mserver_02.uds",
    "/tmp/ipc_testing/ipc_mserver_03.uds",
    "/tmp/ipc_testing/ipc_mserver_04.uds"
};

int main () {
    int sockets[max_connections];
    int max_sock = -1;

    int active_socket, msgsock, rval;

    struct sockaddr_un addresses[max_connections];
    char buf[1024];

    fd_set* readable_sockets = new fd_set;
    fd_set* writeable_sockets = new fd_set;

    FD_ZERO(readable_sockets);
    FD_ZERO(writeable_sockets);

    for (int i = 0; i < max_connections; i++) {

        addresses[i].sun_family = AF_UNIX;
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

        cout << "socket_path -->" << addresses[i].sun_path << endl;

        FD_SET(sockets[i], readable_sockets);
        if (sockets[i] > max_sock) max_sock = sockets[i];
    }

    while(true) {
        active_socket = select(max_sock + 1, readable_sockets, NULL, NULL, NULL);
        cout << "max_sock: " << max_sock << endl;
        cout << "active_socket: " << active_socket << endl;
        if (active_socket < 0) {
            perror("function select() returned trash");
            continue;
        }

        if (FD_ISSET(msgsock, readable_sockets)) {
            msgsock = accept(active_socket, 0, 0);

            if (msgsock == -1) perror("function accept()");
            else do {
                bzero (buf, sizeof(buf));
                if ((rval = read(msgsock, buf, 1024)) < 0) perror("reading stream mesasge");
                else if (rval == 0) cout << "ending connection" << endl;
                else cout << "recived message: " << buf << endl;
            } while (rval > 0);

        }

        close(msgsock);
    }

    for (int i = 0; i < max_connections; i++) {
        close(sockets[i]);
        unlink(SOCKET_FILES[i].c_str());
    }

    FD_ZERO(readable_sockets);
    FD_ZERO(writeable_sockets);

    return 0;

}
