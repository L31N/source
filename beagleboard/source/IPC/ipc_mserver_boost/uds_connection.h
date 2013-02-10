
#ifndef _UDS_CONNECTION_H_
#define _UDS_CONNECTION_H_

#include <iostream>
#include <string>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>

class UdsServer;

class UdsConnection {
    public:
        UdsConnection(boost::asio::io_service& io_service, UdsServer* parent_server);
        ~UdsConnection();

        unsigned short getID();

    private:
        //void handle_read();
        void handle_write();
        void handle_init();
        void write(std::string strdata);

        // ------------------- //

        boost::asio::local::stream_protocol::socket* socket;
        boost::asio::streambuf sbuf;
        //std::string data_string;

        UdsServer* server;

        unsigned short id;
};

#endif // _UDS_CONNECTION_H_
