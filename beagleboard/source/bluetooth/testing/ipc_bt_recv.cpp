
#include "ipc/ipc_connection.h"

#include <string>
#include <iostream>

int main () {
    ipcReceivingConnection ipc_rcon("DEBUG");
    while(true) {
        Data* data = ipc_rcon.readDataFromBuffer();

        while(data != NULL) {
            std::cout << "data: " << data->getData() << std::endl;
            std::cout << "from: " << data->getSenderID() << std::endl;
            std::cout << "\n" << std::endl;

            data = ipc_rcon.readDataFromBuffer();

            usleep(10000);
        }
    }
}
