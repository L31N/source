//
// stream_server.cpp
// ~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdio>
#include <iostream>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

#include "uds_server.h"

#if defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)

/*using boost::asio::local::stream_protocol;

class session
  : public boost::enable_shared_from_this<session>
{
public:
    static int session_count;

public:
  session(boost::asio::io_service& io_service)
    : socket_(io_service)
  {
      num_of_session = session_count;
      session_count++;
  }

  stream_protocol::socket& socket()
  {
    return socket_;
  }

  void start()
  {
        std::cout << "new connection accepted..." << std::endl;
    socket_.async_read_some(boost::asio::buffer(data_),
        boost::bind(&session::handle_read,
          shared_from_this(),
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
  }

  void handle_read(const boost::system::error_code& error,
      size_t bytes_transferred)
  {
    if (!error)
    {
      boost::asio::async_write(socket_,
          boost::asio::buffer(data_, bytes_transferred),
          boost::bind(&session::handle_write,
            shared_from_this(),
            boost::asio::placeholders::error));
        std::cout << "received data(" << num_of_session << "): " << std::string(data_.data(), bytes_transferred) << std::endl;
        //std::cout << "bytes_transferred: " << bytes_transferred << std::endl;
    }
  }

  void handle_write(const boost::system::error_code& error)
  {
    if (!error)
    {
      socket_.async_read_some(boost::asio::buffer(data_),
          boost::bind(&session::handle_read,
            shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    }
  }

private:
  // The socket used to communicate with the client.
  stream_protocol::socket socket_;

  // Buffer used to store data received from the client.
  boost::array<char, 1024> data_;

  int num_of_session;
};

typedef boost::shared_ptr<session> session_ptr;

int session::session_count = 0;





class server
{
public:
  server(boost::asio::io_service& io_service, const std::string& file)
    : io_service_(io_service),
      acceptor_(io_service, stream_protocol::endpoint(file))
  {
    session_ptr new_session(new session(io_service_));
    acceptor_.async_accept(new_session->socket(),
        boost::bind(&server::handle_accept, this, new_session,
          boost::asio::placeholders::error));
  }

  void handle_accept(session_ptr new_session,
      const boost::system::error_code& error)
  {
    if (!error)
    {
      new_session->start();
      new_session.reset(new session(io_service_));
      acceptor_.async_accept(new_session->socket(),
          boost::bind(&server::handle_accept, this, new_session,
            boost::asio::placeholders::error));
    }
  }

private:
  boost::asio::io_service& io_service_;
  stream_protocol::acceptor acceptor_;
};




*/


int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: " << argv[0] << " <UDS_FILE>\n";
      return 1;
    }

    boost::asio::io_service io_service;

    std::remove(argv[1]);
    Server s(io_service, argv[1]);

    io_service.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}

#else // defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)
# error Local sockets not available on this platform.
#endif // defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)
