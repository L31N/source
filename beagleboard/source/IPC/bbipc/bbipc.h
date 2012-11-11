
#ifndef _BB_IPC_H_
#define _BB_IPC_H_

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#include <string>

//const std::string IPC_CONFIG_FILE_PATH = "../../../../../etc/ipc.conf";
const std::string IPC_CONFIG_FILE_PATH = "../etc/ipc.conf";
enum HOST_TYPE {IPC_LOCAL, IPC_BLUETOOTH};

#endif // _BB_IPC_H_
