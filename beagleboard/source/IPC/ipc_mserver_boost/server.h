
#ifndef _SERVER_H_
#define _SERVER_H_

#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

class Server {
    public:
        Server();
        ~Server();

        run();

    private:
        boost::asio::io_service& io_service_;
        boost::asio::local::stream_protocol::acceptor acceptor_;

        boost::asio::local::stream_protocol::socket socket_;

        boost::array<char, 1024> data_;

        std::vector<>
};

#endif // _SERVER_H_
