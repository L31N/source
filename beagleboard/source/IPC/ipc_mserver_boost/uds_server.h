
#ifndef _UDS_SERVER_H_
#define _UDS_SERVER_H_

#include <iostream>
#include <vector>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

class UdsConnection;

class UdsServer {
    public:
        UdsServer(boost::asio::io_service& io_service, boost::asio::local::stream_protocol::endpoint endpoint);
        ~UdsServer();

    private:
        void start_accept();
        void handle_accept(UdsConnection* connection, const boost::system::error_code& error);

        // -------------------- //

        boost::asio::local::stream_protocol::acceptor acceptor;

};

#endif // _UDS_SERVER_H_

