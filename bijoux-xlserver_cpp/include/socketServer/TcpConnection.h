
#ifndef TCP_CONNECTION_H_
#define TCP_CONNECTION_H_

#include "FunctionLoader.h"
#include "protocol/XStream.h"

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>

using boost::asio::ip::tcp;

class TcpConnection
	: public boost::enable_shared_from_this<TcpConnection> {

public:
	typedef boost::shared_ptr<TcpConnection> pointer;

	static pointer create ( boost::asio::io_service& io_service ) {
		return pointer ( new TcpConnection ( io_service ) );
	}

	tcp::socket& socket ( ) {
		return socket_;
	}

	void start ( );

private:
	TcpConnection ( boost::asio::io_service& io_service ) : socket_ ( io_service ) {
	}

	void handle_write ( const boost::system::error_code& /*error*/,
					size_t /*bytes_transferred*/ ) {
	}

	void handleFunctionCall ( XStream &xs );
	void handleLogin ( XStream &xs );

	std::string username_;
	std::string machineName_;
	std::string versionString_;
	std::string protocolVersion_;

	tcp::socket socket_;
};

#endif
