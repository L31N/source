
#ifndef _SPI_MSERVER_H_
#define _SPI_MSERVER_H_

#include <boost/thread/thread.hpp>
#include <sys/mman.h>
#include <sys/stat.h>

#include "ipc/ipc_connection.h"
#include "spimcp2515.h"


class SpiMServer {
    public:
        SpiMServer();
        ~SpiMServer();

        void run();     // blocking funktion, that represents the running process.

    private:
        static void th_recv_fctn(boost::mutex* mtx, Mcp2515* mcp2515);                                  // receives the CAN messages via SPI and redirects them via IPC
        static void th_snd_fctn(boost::mutex* mtx, Mcp2515* mcp2515, ipcReceivingConnection* rcon);     // receives the messages via IPC and redirect them via SPI to CAN

        /** ++++++++++++++++++ **/

        boost::thread* th_recv;
        boost::thread* th_snd;

        boost::mutex mtx;

        ipcReceivingConnection* rcon;
        Mcp2515* mcp2515;
};

#endif // _SPI_MSERVER_H_
