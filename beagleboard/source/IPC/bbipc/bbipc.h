
#ifndef _BB_IPC_H_
#define _BB_IPC_H_

//#define USE_DEBUG_OUTPUT
//#define USE_IPC_CONFIG_DEBUG

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#include <string>

#include "ipc_connection.h"
#include "buffer.h"
#include "ipc_config.h"

const std::string IPC_CONFIG_FILE_PATH = "../../../../../etc/ipc.conf";

#endif // _BB_IPC_H_
