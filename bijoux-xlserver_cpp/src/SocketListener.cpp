
#include "SocketListener.h"

#include "socketServer/TcpServer.h"

bool SocketListener::startListening ( ) {

	try {
		boost::asio::io_service io_service;
		TcpServer server ( io_service );

		io_service.run();
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
		return false;
	}

	return true;
}

bool SocketListener::stopListening ( ) {
	return true;
}
