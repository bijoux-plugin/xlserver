// John Stratoudakis
// johnstratoudakis@gmail.com
//
// File: server_main.cpp
// Description: Entry point into the Bijoux-XLServer application
//

#include <string>
#include <vector>

#include "FunctionLoader.h"
#include "SocketListener.h"

#include <log4cxx/logger.h>
#include <log4cxx/xml/domconfigurator.h>

using namespace log4cxx;
using namespace log4cxx::xml;
using namespace log4cxx::helpers;

LoggerPtr logMain(Logger::getLogger( "main"));

int main ( void ) {

	// Load XML configuration file using DOMConfigurator
	DOMConfigurator::configure("./config/Log4cxxConfig.xml");

	LOG4CXX_INFO ( logMain, "Bijoux-XlServer started " );

	// Name each class that will export functions here
	std::vector< std::string > allClasses;
	allClasses.push_back ( "General" );
	allClasses.push_back ( "American" );

	FunctionLoader::loadAllFunctions ( allClasses );

	SocketListener socketListener;

	if ( socketListener.startListening ( ) == false ) {
		LOG4CXX_ERROR ( logMain, "Error calling SocketListener.startListening()." );
		LOG4CXX_ERROR ( logMain, "Exiting..." );
		return ( -1 );
	}

	std::string userInput;
	while ( true ) {
		std::cout << "\n\nEnter Q/q to stop the server.\n\n";
		std::cout.flush();

		std::cin >> userInput;
		std::cout << "\nYou entered: " << userInput;
		if ( userInput.at ( 0 ) == 'q' || userInput.at ( 0 ) == 'Q' ) {
			std::cout << "\nExiting...";
			break;
		}
	}

	socketListener.stopListening();

	return ( 0 );
}
