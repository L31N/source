
#ifndef _SPI_MSERVER_H_
#define _SPI_MSERVER_H_

#include <boost/thread/thread.hpp>
#include "ipc/ipc_connection.h"
#include "spimcp2515.h"


class SpiMServer {
    public:
        SpiMServer();
        ~SpiMServer();

    private:
        static void th_recv_fctn();
        static void th_snd_fctn();

        /** ++++++++++++++++++ **/

        boost::thread* th_recv;
        boost::thread* th_snd;

        ipcReceivingConnection* rcon;
        Mcp2515* mcp2515;
};

#endif // _SPI_MSERVER_H_
