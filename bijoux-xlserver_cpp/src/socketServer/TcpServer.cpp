
#include "socketServer/TcpServer.h"

void TcpServer::start_accept ( ) {

	TcpConnection::pointer new_connection = TcpConnection::create ( acceptor_.get_io_service ( ) );

	acceptor_.async_accept ( new_connection->socket ( ),
		boost::bind ( &TcpServer::handle_accept, this, new_connection,
			boost::asio::placeholders::error) );
}

void TcpServer::handle_accept ( TcpConnection::pointer new_connection, const boost::system::error_code& error ) {

	if ( !error ) {
		new_connection->start();
	}

	start_accept();
}
