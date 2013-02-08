
#ifndef _UDS_SERVER_H_
#define _UDS_SERVER_H_

#include <cstdio>
#include <iostream>
#include <vector>
#include <iterator>

#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

class Session;
class Server;

typedef boost::shared_ptr<Session> session_ptr;

class Session : public boost::enable_shared_from_this<Session> {
    public:
        Session(boost::asio::io_service& io_service);
        ~Session();

        boost::asio::local::stream_protocol::socket& socket();

        void start();
        void handle_read(const boost::system::error_code& error, size_t bytes_transferred);
        void handle_write(const boost::system::error_code& error);
        void close();


    private:
        // The socket used to communicate with the client.
        boost::asio::local::stream_protocol::socket socket_;

        // Buffer used to store data received from the client.
        boost::array<char, 1024> data_;

        // Only to identify the connection.
        int num_of_session;

    public:
        static int session_count;

};

class Server {
    public:
        Server(boost::asio::io_service& io_service, const std::string& file);
        ~Server();

        void handle_accept(session_ptr new_session, const boost::system::error_code& error);
        void handle_close(std::iterator it);


    private:
        boost::asio::io_service& io_service_;
        boost::asio::local::stream_protocol::acceptor acceptor_;

        std::vector<session_ptr> sessions;
        std::iterator<random_access_iterator_tag, session_ptr> session_iterator;
};

#endif // _UDS_SERVER_H_
