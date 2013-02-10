
// uds_connection.cpp

#include <sstream>

#include "uds_connection.h"
#include "uds_server.h"

UdsConnection::UdsConnection(boost::asio::io_service& io_service, UdsServer* pserver) : server(pserver), socket(new boost::asio::local::stream_protocol::socket(io_service)) {
    std::cout << "UdsConnection::UdsConnection()" << std::endl;
}

UdsConnection::~UdsConnection() {
    std::cout << "UdsConnection::~UdsConnection()" << std::endl;
    socket->shutdown(boost::asio::socket_base::shutdown_both);
    socket->close();
}

boost::asio::local::stream_protocol::socket& UdsConnection::getSocket() { return *socket; }

void UdsConnection::start() {
    std::cout << "UdsConnection::start() ..." << std::endl;
    cbuf = new char[3];
    // listen for authentification //
    boost::asio::async_read(*socket, boost::asio::buffer(cbuf, 3),
                            boost::bind(&UdsConnection::handle_init, this
                                        )
                            );
}

void UdsConnection::handle_init() {
    std::cout << "UdsConnection::handle_init()" << std::endl;

    /** extract the id and package-size **/

    sender_id = cbuf[0];
    endpoint_id = cbuf[1];
    package_size = cbuf[2];

    /** **/

    delete cbuf;
    cbuf = new char[package_size];

    std::cout << "sender_id: " << sender_id << std::endl;
    std::cout << "endpoint_id: " << endpoint_id << std::endl;
    std::cout << "package_size: " << package_size << std::endl;

    if (sender_id == endpoint_id) {
        server->registerConnection(this, endpoint_id);
    }

    this->listen();
}

void UdsConnection::listen() {
    boost::asio::async_read(*socket, boost::asio::buffer(cbuf, package_size),
                            boost::bind(&UdsConnection::handle_received, this
                                        )
                            );
}

void UdsConnection::handle_received() {
    // create the final string to redirect
    std::string data_to_send (1, char(sender_id));
std::cout << data_to_send[0] << std::endl;
    //data_to_send += std::string(cbuf, package_size);

    std::cout << "sender_id: " << sender_id << std::endl;
    std::cout << "data: " << cbuf << std::endl;

    std::cout << "data_to_send[0]: " <<  int(data_to_send[0]) << std::endl;
    std::cout << "data_to_send: " << data_to_send << std::endl;

    server->send(endpoint_id, data_to_send);
}

void UdsConnection::write(std::string data) {
    std::cout << "UdsConnection::write()" << std::endl;
    boost::asio::async_write(*socket, boost::asio::buffer(data),
                             boost::bind(&UdsConnection::handle_write, this));
}

void UdsConnection::handle_write() {
    std::cout << "UdsConnection::handle_write()" << std::endl;
}
