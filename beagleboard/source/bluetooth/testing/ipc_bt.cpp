
#include "ipc/ipc_connection.h"

#include <string>
#include <iostream>

int main () {
    ipcSendingConnection ipc_scon ("TACTIC_MODULE", "DEBUG", IPC_BLUETOOTH);
    ipc_scon.sendData("testmessage");

    return 0;
}
