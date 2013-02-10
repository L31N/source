
#ifndef _UDS_SERVER_H_
#define _UDS_SERVER_H_

#include <iostream>
#include <vector>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

class UdsConnection;

struct Connection {
    UdsConnection* connection;
    unsigned short id;
};

class UdsServer {
    public:
        UdsServer(boost::asio::io_service& io_service, boost::asio::local::stream_protocol::endpoint endpoint);
        ~UdsServer();

        void registerConnection(UdsConnection* _connection, unsigned short _endpoint_id);
        void send(unsigned short endpoint_id, std::string data);

    private:
        void start_accept();
        void handle_accept(UdsConnection* connection, const boost::system::error_code& error);

        // -------------------- //

        boost::asio::local::stream_protocol::acceptor acceptor;

        std::vector<Connection> rcons;

};

#endif // _UDS_SERVER_H_

