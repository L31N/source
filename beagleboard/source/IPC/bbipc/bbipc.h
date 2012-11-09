
#ifndef _BB_IPC_H_
#define _BB_IPC_H_

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#include <string>

const std::string IPC_CONFIG_FILE_PATH = "../../../../../etc/ipc.conf";
enum HOST_TYPE {IPC_LOCAL, IPC_BLUETOOTH, IPC_SHARED};

//#include "ipc_connection.h"
//#include "buffer.h"
//#include "ipc_config.h"

#endif // _BB_IPC_H_
