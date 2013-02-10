
#ifndef _UDS_SERVER_H_
#define _UDS_SERVER_H_

#include <iostream>
#include <vector>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>


class UdsConnection;

class UdsServer {
    public:
        UdsServer(boost::asio::io_service& io_service, boost::asio::local::stream_protocol::endpoint _endpoint);
        ~UdsServer();

        void handle_received(boost::asio::streambuf data, unsigned short id);     // called fromout the UdsConnection class.

    private:
        void start_accept();
        void handle_accept(UdsConnection* new_connection, const boost::system::error_code& error);

        // ----------------------- //

        boost::asio::local::stream_protocol::endpoint* endpoint;
        boost::asio::local::stream_protocol::acceptor* acceptor;

        std::vector<UdsConnection*> connections;
};

#endif // _UDS_SERVER_H_
