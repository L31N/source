
#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include <string>

#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

class Connection() {
    public:
        Connection();
        ~Connection();

        send(std::string data);

    private:
        unsigned short id;
        boost::asio::local::stream_protocol::socket sock;
}

#endif // _CONNECTION_H_

