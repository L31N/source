
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

        enum callback {
            setup_successfully = 1,
            setup_failed = 0,
            delivered_successfully = 3,
            delivered_failed = 4
        };

        boost::asio::local::stream_protocol::socket& getSocket();

        void start();
        bool write(std::string data);
        void send_callback(UdsConnection::callback callback);

    private:
        void handle_init(const boost::system::error_code& error);
        void listen();
        void handle_received(const boost::system::error_code& error);

        // ----------------------- //

        UdsServer* server;
        boost::asio::local::stream_protocol::socket* socket;

        unsigned short sender_id;
        unsigned short endpoint_id;

        unsigned short package_size;

        char* cbuf;
};

#endif // _UDS_CONNECTION_H_
