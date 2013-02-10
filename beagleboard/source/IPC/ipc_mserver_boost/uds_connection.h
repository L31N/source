
#ifndef _UDS_CONNECTION_H_
#define _UDS_CONNECTION_H_

#include <iostream>
#include <string>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

class UdsServer;

class UdsConnection {
    public:
        UdsConnection(boost::asio::io_service& io_service, UdsServer* pserver);
        ~UdsConnection();

        boost::asio::local::stream_protocol::socket& getSocket();

        void start();
        void write(std::string data);

    private:
        void handle_init();
        void listen();
        void handle_received();

        // ----------------------- //

        UdsServer* server;
        boost::asio::local::stream_protocol::socket* socket;

        unsigned short sender_id;
        unsigned short endpoint_id;

        unsigned short package_size;

        char* cbuf;
};

#endif // _UDS_CONNECTION_H_
