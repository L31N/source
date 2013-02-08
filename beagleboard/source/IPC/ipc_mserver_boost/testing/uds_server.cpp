
// uds_server.cpp

#include "uds_server.h"

/** CLASS SESSION **/

Session::Session(boost::asio::io_service& io_service) : socket_(io_service) {
    num_of_session = session_count;
    session_count++;
}

Session::~Session() {
    //session_count --;
}

boost::asio::local::stream_protocol::socket& Session::socket() { return socket_; }

void Session::start() {
    std::cout << "new connection accepted..." << std::endl;
    socket_.async_read_some(boost::asio::buffer(data_),
                            boost::bind(&Session::handle_read,
                                        shared_from_this(),
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred
                                        )
                            );
}

void Session::handle_read(const boost::system::error_code& error, size_t bytes_transferred) {
    if (!error) {
        boost::asio::async_write(socket_,
                                 boost::asio::buffer(data_, bytes_transferred),
                                 boost::bind(&Session::handle_write,
                                             shared_from_this(),
                                             boost::asio::placeholders::error)
                                 );

        std::cout << "received data(" << num_of_session << "): " << std::string(data_.data(), bytes_transferred) << std::endl;
    }
}

void Session::handle_write(const boost::system::error_code& error) {
    if (!error) {
        socket_.async_read_some(boost::asio::buffer(data_),
                                boost::bind(&Session::handle_read,
                                            shared_from_this(),
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::bytes_transferred)
                                );
    }
}

void Session::close() {
    //boost::system::error_code ec;
    socket_.shutdown(boost::asio::socket_base::shutdown_both, boost::system::error_code ec);
    socket_.close();
}

int Session::session_count = 0;






/** CLASS SERVER **/

Server::Server(boost::asio::io_service& io_service, const std::string& file) : io_service_(io_service), acceptor_(io_service, boost::asio::local::stream_protocol::endpoint(file)) {
    session_ptr new_session(new Session(io_service_));
    acceptor_.async_accept(new_session->socket(),
                           boost::bind(&Server::handle_accept, this, new_session,
                                       boost::asio::placeholders::error)
                           );

    sessions.clear();
}

Server::~Server() {}

void Server::handle_accept(session_ptr new_session, const boost::system::error_code& error) {
    if (!error) {
        new_session->start();
        new_session.reset(new Session(io_service_));
        acceptor_.async_accept(new_session->socket(),
                               boost::bind(&Server::handle_accept, this, new_session,
                                           boost::asio::placeholders::error)
                               );
        sessions.push_back(new_session);
        std::cout << "added new session ..." << std::endl;
        std::cout << "vector-size: " << sessions.size() << std::endl;
    }
}


void Server::handle_close(std::iterator it) {
    sessions.erase(it);
    std::cout << "deleted session ..." << std::endl;
    std::cout << "vector-size: " << sessions.size() << std::endl;
}
