
#include <iostream>
#include "ipc/ipc_connection.h"

/*int main () {
    ipcSendingConnection scon("DEBUG", "SENSOR_API");

    if (scon.is_open()) {
        scon.sendData("helloworld");
    }
    else {
        std::cerr << "error" << std::endl;
    }

    return 0;
}*/

int main () {
    ipcReceivingConnection rcon("SENSOR_API");

    if (rcon.is_open()) {
        std::string data = rcon.readDataFromBuffer()->getData();
        std::cout << "data: " << data << std::endl;
    }
    else {
        std::cerr << "error" << std::endl;
    }

    return 0;
}

/*#include "ipc/ipc_connection.h"

int main (int argc, char** argv) {
    if (argc <= 1) {
        std::cout << "usage: " << argv[0] << " [read | write]" << std::endl;
        return -1;
    }
    else if (argv[1] == std::string("write")) {
        ipcSendingConnection scon(1, 2);

        if (scon.is_open()) {
            while(true) {
                char insertion = 0;
                std::cout << "please insert sth: "; std::cin >> insertion;
                scon.sendData(std::string(&insertion));
            }
        }
        else {
            std::cerr << "could not open ipcSendingConnection" << std::endl;
            return -1;
        }
    }
    else if (argv[1] == std::string("read")) {
        ipcReceivingConnection rcon(2);

        if (rcon.is_open()) {
            std::cout << "reading data ..." << std::endl;
            while(true) {
                std::string data;
                Data* dp = NULL;
                if (rcon.checkForNewData()) {
                    dp = rcon.readDataFromBuffer();
                    if (dp != NULL) {
                        data = dp->getData();
                        std::cout << "received data: [" << data << "]" << std::endl;
                    }
                    else std::cout << ".";
                }
                usleep(100);
            }
        }
        else {
            std::cerr << "could not open ipcReceivingConnection" << std::endl;
            return -1;
        }
    }

    else {
        std::cerr << "recognized illegal argument: \"" << argv[1] << "\"" << std::endl;
        return -1;
    }

    return 0;
}*/
