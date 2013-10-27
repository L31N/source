
// uds_connection.cpp

#include <sstream>

#include "uds_connection.h"
#include "uds_server.h"

UdsConnection::UdsConnection(boost::asio::io_service& io_service, UdsServer* pserver) : server(pserver), socket(new boost::asio::local::stream_protocol::socket(io_service)) {
    //std::cout << "UdsConnection::UdsConnection()" << std::endl;
}

UdsConnection::~UdsConnection() {
    //std::cout << "UdsConnection::~UdsConnection()" << std::endl;
    socket->shutdown(boost::asio::socket_base::shutdown_both);
    socket->close();

    delete socket;
    delete cbuf;
}

boost::asio::local::stream_protocol::socket& UdsConnection::getSocket() { return *socket; }

void UdsConnection::start() {
    //std::cout << "UdsConnection::start() ..." << std::endl;
    cbuf = new char[3];
    // listen for authentification //
    boost::asio::async_read(*socket, boost::asio::buffer(cbuf, 3),
                            boost::bind(&UdsConnection::handle_init, this,
                                        boost::asio::placeholders::error
                                        )
                            );
}

void UdsConnection::handle_init(const boost::system::error_code& error) {
    //std::cout << "UdsConnection::handle_init()" << std::endl;

    if (!error) {
        /** extract the id and package-size **/

        sender_id = cbuf[0];
        endpoint_id = cbuf[1];
        package_size = cbuf[2];

        /** **/

        delete cbuf;
        cbuf = new char[package_size + 1];

        //std::cout << "sender_id: " << sender_id << std::endl;
        //std::cout << "endpoint_id: " << endpoint_id << std::endl;
        //std::cout << "package_size: " << package_size << std::endl;

        if (sender_id == endpoint_id) {
            server->registerConnection(this, endpoint_id);
        }
        else {
            this->listen();
            this->send_callback(UdsConnection::setup_successfully);
        }
    }
    else {
        std::cerr << "ERROR: UdsConnection::handle_init() --> boost::system::error_code (client may be closed): " << error.message() << std::endl;
    }
}

void UdsConnection::listen() {
    //std::cout << "UdsConnection::listen()" << std::endl;
    //std::cout << "package_size: " << package_size << std::endl;

    delete cbuf;
    cbuf = new char[package_size + 1];

    boost::asio::async_read(*socket, boost::asio::buffer(cbuf, package_size + 1),  // +1 is the bluetooth byte
                            boost::bind(&UdsConnection::handle_received, this,
                                        boost::asio::placeholders::error)
                            );
}

void UdsConnection::handle_received(const boost::system::error_code& error) {
    //std::cout << "\tUdsConnection::handle_received()" << std::endl;
    //std::cout << "received message: " << std::string(cbuf, package_size + 1) << std::endl;

    if (!error) {
        // create the final string to redirect
        std::string data_to_send (1, char(sender_id));
        data_to_send += std::string(cbuf, package_size + 1);

        //std::cout << "data_to_send: " << data_to_send << std::endl;

        server->send(this, endpoint_id, data_to_send);
        this->listen();
    }
    else {
        //std::cerr << "ERROR: UdsConnection::handle_received() --> boost::system::error_code (client may be closed): " << error.message() << std::endl;
    }
}

bool UdsConnection::write(std::string data) {
    //std::cout << "UdsConnection::write()" << std::endl;
    //std::cout << "data to wirte: " << data << std::endl;

    std::string tmp_str(data);

    boost::system::error_code error;
    boost::asio::write(*socket, boost::asio::buffer(tmp_str), error);

    if (error) {
        std::cerr << "ERROR: UdsConnection::write() --> boost::system::error_code: " << error.message() << std::endl;
        //server->releaseConnection(this);
        return false;
    }
    else return true;
}

void UdsConnection::send_callback(UdsConnection::callback callback) {
    //std::cout << "UdsConnection::send_callback()" << std::endl;
    //std::cout << "callback: " << int(callback) << std::endl;

    boost::system::error_code error;
    boost::asio::write(*socket, boost::asio::buffer(std::string(1, char(callback))), error);

    if (error) {
        std::cerr << "ERROR: UdsConnection::write() --> boost::system::error_code" << std::endl;
    }
}

