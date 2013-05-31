
#ifndef TCP_SERVER_H_
#define TCP_SERVER_H_

#include "socketServer/TcpConnection.h"
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class TcpServer {

public:
	TcpServer ( boost::asio::io_service & io_service )
			: acceptor_ ( io_service, tcp::endpoint ( tcp::v4(), 6000 ) ) {
			start_accept ( );
		}

private:

	void start_accept ( );
	void handle_accept ( TcpConnection::pointer new_connection, const boost::system::error_code& error );

	tcp::acceptor acceptor_;
};

#endif
