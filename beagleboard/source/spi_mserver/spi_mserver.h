
#ifndef _SPI_MSERVER_H_
#define _SPI_MSERVER_H_

#include <boost/thread/thread.hpp>
#include <sys/mman.h>
#include <sys/stat.h>

#include <errno.h>
#include <signal.h>

#include <functional>

#include "ipc/ipc_connection.h"
#include "spimcp2515.h"

#include "gpio.h"

class SpiMServer {
    public:
        SpiMServer();
        ~SpiMServer();

        void run();     // blocking funktion, that represents the running process.

    private:
        static void th_recv_fctn(boost::mutex* mtx, Mcp2515* mcp2515, int* gpio_fd);                                  // receives the CAN messages via SPI and redirects them to IPC
        static void th_snd_fctn(boost::mutex* mtx, Mcp2515* mcp2515, ipcReceivingConnection* rcon);     // receives the messages via IPC and redirect them via SPI to CAN

        static void close_handler(int signum);

        /** ++++++++++++++++++ **/

        boost::thread* th_recv;
        boost::thread* th_snd;

        boost::mutex mtx;

        ipcReceivingConnection* rcon;
        Mcp2515* mcp2515;

        int gpio_fd;

        static const unsigned short gpio = 139;
};

#endif // _SPI_MSERVER_H_
