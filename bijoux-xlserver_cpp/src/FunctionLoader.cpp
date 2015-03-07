
#include "FunctionLoader.h"

#include <boost/foreach.hpp>

#include <log4cxx/logger.h>
#include <log4cxx/xml/domconfigurator.h>

using namespace log4cxx;
using namespace log4cxx::xml;
using namespace log4cxx::helpers;

func_map * FunctionLoader::all_functions = NULL;

LoggerPtr loggerFunctionLoader ( Logger::getLogger( "FunctionLoader" ) );

void FunctionLoader::loadFunctions ( std::string className, func_map& f_map ) {

	using namespace ROOT::Reflex;

	LOG4CXX_INFO ( loggerFunctionLoader, "FunctionLoader->loadFunctions ( " << className << " ) finished." );
	//LOG4CXX_INFO ( loggerFunctionLoader, "FunctionLoader->loadFunctions ( " << className << " ) started" );

	Type myType = Type::ByName ( className );
	int numberOfMembers = myType.FunctionMemberSize ( INHERITEDMEMBERS_NO );

	// FunctionMember_Begin
	for ( int i=0; i < numberOfMembers; i++ ) {
		Member member = myType.FunctionMemberAt ( i, INHERITEDMEMBERS_NO );

		if ( member.IsStatic ( ) == true ) {
			AnnotationList annotationList = member.Annotations();
			if ( annotationList.AnnotationSize() > 0 ) {
				if ( annotationList.HasAnnotation ( typeid(ExcelFunction) ) ) {
					const ExcelFunction* func = annotationList.AnnotationWithType<ExcelFunction> ( );

//					std::cout << "\n";
//					std::cout << "\n\t\tfunc.name: " << func->getName();
//					std::cout << "\n\t\tfunc.help: " << func->getHelp ();
//					std::cout << "\n\t\tfunc.category: " << func->getCategory ();
//					std::cout << "\n\t\tfunc.argHelp: " << func->getArgHelp ();

					std::stringstream functionName;
					functionName << func->getCategory() << "." << func->getName();
					f_map [ functionName.str() ] = std::make_pair< ROOT::Reflex::Member, ExcelFunction > ( member, *func );

					LOG4CXX_INFO ( loggerFunctionLoader, "\t\tLoaded function: \t" << functionName.str() );
/*
                         int result = 0;
                         std::vector<void *> parameters;
                         int age_parm = 33;
                         float mul_parm = 0.5;
                         parameters.push_back ( &age_parm );
                         parameters.push_back ( &mul_parm );
                         member.Invoke <int &> ( result, parameters );
                         std::cout << "\nResult of invoking: " << result;
                         std::cout << "\nPlus 7 = " << result + 7;
*/
				}
			}
		}
	}

}

void FunctionLoader::loadAllFunctions ( std::vector < std::string > allClasses ) {

	LOG4CXX_INFO ( loggerFunctionLoader, "FunctionLoader->loadAllFunctions() started" );

	func_map *f_map = new func_map;

	for ( std::vector<std::string>::iterator it = allClasses.begin() ; it != allClasses.end() ; ++ it ) {
		FunctionLoader::loadFunctions ( *it, *f_map );
	}

	FunctionLoader::all_functions = f_map;

	LOG4CXX_INFO ( loggerFunctionLoader, "FunctionLoader->loadAllFunctions() finished" );
}

void FunctionLoader::dumpAllFunctions ( ) {

	func_map *f_map = FunctionLoader::getAllfunctionsPtr ( );

	LOG4CXX_INFO ( loggerFunctionLoader, "+------------------------------------------------------------+" );
	LOG4CXX_INFO ( loggerFunctionLoader, "Printing out all members via f_map" );

	BOOST_FOREACH ( func_map::value_type val, *f_map ) {

		std::string function_name = val.first;

		std::pair < ROOT::Reflex::Member, ExcelFunction > & value_pair = val.second;
		ROOT::Reflex::Member & member = value_pair.first;
		ExcelFunction & excelFunction = value_pair.second;

		LOG4CXX_INFO ( loggerFunctionLoader, "" );
		LOG4CXX_INFO ( loggerFunctionLoader, "\tExposed function name: " << function_name );
		LOG4CXX_INFO ( loggerFunctionLoader, "\tFunction Name: " << excelFunction.getName ( ) );
		LOG4CXX_INFO ( loggerFunctionLoader, "\tFunction Help: " << excelFunction.getHelp ( ) );
		LOG4CXX_INFO ( loggerFunctionLoader, "\tFunction Category: " << excelFunction.getCategory ( ) );
		//LOG4CXX_INFO ( loggerFunctionLoader, "\tArgument Help: " << excelFunction.getArgHelp ( ) );

		LOG4CXX_INFO ( loggerFunctionLoader, "" );;
		LOG4CXX_INFO ( loggerFunctionLoader, "MemberTypeAsString: " << member.MemberTypeAsString() );
		LOG4CXX_INFO ( loggerFunctionLoader, "Name: " << member.Name() );
		LOG4CXX_INFO ( loggerFunctionLoader, "# of parameters - FunctionParameterSize: " << member.FunctionParameterSize() );
		// # of parameters is important
		// Return type would be nice but is not necessary because we should only be using LPXLOPER variables
		LOG4CXX_INFO ( loggerFunctionLoader, "Return type: " );
	}
}

