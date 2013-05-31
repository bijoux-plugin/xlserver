
#include "socketServer/TcpConnection.h"

#include "XLCall.h"
#include "protocol/XLUtil.h"
#include "protocol/XLCodec.h"
#include "XLCodecCommon.h"
#include "FunctionCaller.h"

#include <boost/foreach.hpp>
#include <boost/algorithm/string/join.hpp>

#include <log4cxx/logger.h>
#include <log4cxx/xml/domconfigurator.h>

using namespace log4cxx;
using namespace log4cxx::xml;
using namespace log4cxx::helpers;

using boost::asio::ip::tcp;

LoggerPtr logTcp( Logger::getLogger( "TcpConnection") );

void TcpConnection::handleFunctionCall ( XStream &xs ) {

	LPXLOPER lpFunctionName = new XLOPER;
	LPXLOPER lpNumberOfParameters = new XLOPER;

	XLCodec::decode ( xs, lpFunctionName );
	XLCodec::decode ( xs, lpNumberOfParameters );

	std::string function_name ( XLUtil::getXlOperValue ( lpFunctionName ) ) ;

	LOG4CXX_INFO ( logTcp, "" );
	LOG4CXX_INFO ( logTcp, "---------------" );
	LOG4CXX_INFO ( logTcp, "Function call: " );
	LOG4CXX_INFO ( logTcp, "\tFunction Name: " << function_name ) ;
	LOG4CXX_INFO ( logTcp, "\t[type] " << XLUtil::getXlOperType ( lpNumberOfParameters ) );
	LOG4CXX_INFO ( logTcp, "\t# of Parameters: " << XLUtil::getXlOperValue ( lpNumberOfParameters ) );

	LPXLOPER *lpArrayOfParameters = new LPXLOPER[ lpNumberOfParameters->val.w ];

	// Prepare source parameters
	std::vector<LPXLOPER> parameters;
	for ( int i = 0; i < lpNumberOfParameters->val.w; i++ ) {
		LPXLOPER lpParm = new XLOPER;
		XLCodec::decode ( xs, lpParm );
		lpArrayOfParameters [ i ] = lpParm;
		parameters.push_back ( lpParm ) ;
		LOG4CXX_INFO ( logTcp, "\t [" << i << "] = " << XLUtil::getXlOperType ( lpArrayOfParameters [ i ] ) );
		LOG4CXX_INFO ( logTcp, "\t [" << i << "] = " << XLUtil::getXlOperValue ( lpArrayOfParameters [ i] ) );
	}

	func_map * f_map = FunctionLoader::getAllfunctionsPtr ( );
	if ( f_map == NULL ) {
		LOG4CXX_ERROR ( logTcp, "f_map is NULL!!!" );
	} else {
		if ( function_name == "org.boris.xlloop.GetFunctions" ) {
			// Special handling of GetFunctions here

			int total_func = 0;
			BOOST_FOREACH ( func_map::value_type val, *f_map ) {
				std::pair<ROOT::Reflex::Member, ExcelFunction >	my_pair = val.second;
				ExcelFunction func = my_pair.second;
				total_func ++;
			}

			// Send out total_func
			XLOPER xlArray;
			xlArray.xltype = xltypeMulti;
			xlArray.val.array.rows = total_func;
			xlArray.val.array.columns = 1;
			xlArray.val.array.lparray = new XLOPER[total_func];
			int i=0;

			BOOST_FOREACH ( func_map::value_type val, *f_map ) {
				std::string function_name = val.first;
				std::pair<ROOT::Reflex::Member, ExcelFunction >	my_pair = val.second;
				ExcelFunction func = my_pair.second;
//		std::cout << "\nExcelFunction information: " << std::endl;
//		std::cout << "\n\tName: " << func.getName ( );
//		std::cout << "\n\tHelp: " << func.getHelp ( );
//		std::cout << "\n\tCategory: " << func.getCategory ( );
//		std::cout << "\n\tArgument Help: " << func.getArgHelp ( );

				int num_of_keys = 6;

				xlArray.val.array.lparray[i].xltype = xltypeMulti;
				xlArray.val.array.lparray[i].val.array.rows = num_of_keys;
				xlArray.val.array.lparray[i].val.array.columns = 2;
				xlArray.val.array.lparray[i].val.array.lparray = new XLOPER [ num_of_keys * 2 ];

				// Set Function Fields
				XLUtil::MakeString ( &xlArray.val.array.lparray[i].val.array.lparray[0], "functionName" );
				XLUtil::MakeString ( &xlArray.val.array.lparray[i].val.array.lparray[1], func.getName().c_str() );

				XLUtil::MakeString ( &xlArray.val.array.lparray[i].val.array.lparray[2], "category" );
				XLUtil::MakeString ( &xlArray.val.array.lparray[i].val.array.lparray[3], func.getCategory().c_str() );

				XLUtil::MakeString ( &xlArray.val.array.lparray[i].val.array.lparray[4], "functionHelp" );
				XLUtil::MakeString ( &xlArray.val.array.lparray[i].val.array.lparray[5], func.getHelp().c_str() );

				XLUtil::MakeString ( &xlArray.val.array.lparray[i].val.array.lparray[6], "isVolatile" );
				XLUtil::MakeBoolean ( &xlArray.val.array.lparray[i].val.array.lparray[7], func.getIsVolatile() );

				XLUtil::MakeString ( &xlArray.val.array.lparray[i].val.array.lparray[8], "argumentText" );
				std::string argsString = boost::algorithm::join ( func.getArgs(), ", " );
				XLUtil::MakeString ( &xlArray.val.array.lparray[i].val.array.lparray[9], argsString.c_str() );

				XLUtil::MakeString ( &xlArray.val.array.lparray[i].val.array.lparray[10], "argumentHelp" );
				std::vector<std::string> argHelp = func.getArgHelp ( );

				LPXLOPER lpArray;
				lpArray = &xlArray.val.array.lparray[i].val.array.lparray[11];

				lpArray->xltype = xltypeMulti;
				lpArray->val.array.rows = 1;
				lpArray->val.array.columns = argHelp.size();
				lpArray->val.array.lparray = new XLOPER [ 1 * argHelp.size() ];
				for ( int i=0; i < argHelp.size(); i++ ) {
					XLUtil::MakeString ( &lpArray->val.array.lparray[i], argHelp.at(i).c_str()  );
				}
				i++;
			}

			XLCodec::encode ( &xlArray, xs );
			xs.flush ();

		} else {
			// Lookup and call the appropriate function here
			LOG4CXX_INFO ( logTcp, "Looking up function: " << XLUtil::getXlOperValue ( lpFunctionName ) );
			bool functionFound = (*f_map).find ( std::string ( XLUtil::getXlOperValue ( lpFunctionName ) ) ) != (*f_map).end();

			if ( functionFound == true ) {
				LOG4CXX_INFO ( logTcp, "Function found" );
				std::pair < ROOT::Reflex::Member, ExcelFunction > & val = (*f_map) [ XLUtil::getXlOperValue ( lpFunctionName ) ];
	
				// Prepare return value
				LPXLOPER retVal = new XLOPER;
				bool result = FunctionCaller::CallFunction ( f_map, function_name, retVal, parameters ) ;
				if ( result == true ) {
					XLCodec::encode ( retVal, xs );
					xs.flush ();
				}
			} else {
				LOG4CXX_INFO ( logTcp, "Function not found" );
				XLOPER xlResult;
				XLUtil::MakeString ( &xlResult, "Function not found!" );
				XLCodec::encode ( &xlResult, xs );
				xs.flush ( );
			}
		}
	}
}

void TcpConnection::handleLogin ( XStream &xs ) {

	LPXLOPER lpXlOper = new XLOPER;

	// Read 4 parameters
	XLCodec::decode ( xs, lpXlOper );
	username_ = XLUtil::getXlOperValue ( lpXlOper );

	XLCodec::decode ( xs, lpXlOper );
	machineName_ = XLUtil::getXlOperValue ( lpXlOper );

	XLCodec::decode ( xs, lpXlOper );
	versionString_ = XLUtil::getXlOperValue ( lpXlOper );

	XLCodec::decode ( xs, lpXlOper );
	protocolVersion_ = XLUtil::getXlOperValue ( lpXlOper );

	LOG4CXX_INFO ( logTcp, "-" );
	LOG4CXX_INFO ( logTcp, "---------------------------" );
	LOG4CXX_INFO ( logTcp, "Creating new session with: " );
	LOG4CXX_INFO ( logTcp, "\tUser Name: " << username_ );
	LOG4CXX_INFO ( logTcp, "\tMachine Name: " << machineName_ );
	LOG4CXX_INFO ( logTcp, "\tVersion String: " << versionString_ );
	LOG4CXX_INFO ( logTcp, "\tProtocol Version: " << protocolVersion_ );

	// Write back a server message
	LPXLOPER lpServerMessage;
	lpServerMessage = XLUtil::MakeExcelString2 ( "Successfully Connected to Bijoux-XlServer" );
	XLCodec::encode ( lpServerMessage, xs );

	// Write back with a boolean "true"
	LPXLOPER lpXlTrue = XLUtil::MakeBoolean ( true );
	XLCodec::encode ( lpXlTrue, xs );
	xs.flush();

	LOG4CXX_INFO ( logTcp, "Successful Login" );
	LOG4CXX_INFO ( logTcp, "---------------------------" );
}

void TcpConnection::start ( ) {

	LOG4CXX_INFO ( logTcp, "TcpConnection - Connected Started." );
	LOG4CXX_INFO ( logTcp, "Thread Id: " << boost::this_thread::get_id ( ) );

	XStream xs ( socket_ );

	handleLogin ( xs );

	LOG4CXX_INFO ( logTcp, "Entering \"Function Mode\"" );
	do {
		try {
			handleFunctionCall ( xs );
		} catch ( std::exception & e ) {
			std::cerr << e.what() << std::endl;
			std::cerr << "Connection closed";
			break;
		}
	} while ( true );
}
