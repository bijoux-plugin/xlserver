
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#define LOG4CXX_INFO(logger, expr)

#include <boost/test/unit_test.hpp>
#include <boost/foreach.hpp>

#include "protocol/XLUtil.h"
#include "FunctionLoader.h"

struct LookupHelper {

	func_map * f_map = NULL;

	LookupHelper ( )  {
		boost::unit_test::unit_test_log.set_threshold_level ( boost::unit_test::log_messages );
		BOOST_TEST_MESSAGE ( "" );

		std::vector< std::string > allClassNames;
		allClassNames.push_back ( "TestClass" );

		FunctionLoader::loadAllFunctions ( allClassNames );
		f_map = FunctionLoader::getAllfunctionsPtr ( );
	}

	~LookupHelper ( ) {
	}
};

BOOST_FIXTURE_TEST_SUITE ( FunctionLoaderTests, LookupHelper )

BOOST_AUTO_TEST_CASE ( lookupUnknownFunction ) {
	BOOST_TEST_MESSAGE ( "TEST: lookupUnknownFunction" );
	BOOST_TEST_MESSAGE ( "Testing the behavior of looking up a function that does not exists." );
	bool functionFound = (*LookupHelper::f_map).find ( std::string("unknownFunction" ) ) != (*LookupHelper::f_map).end();
	BOOST_CHECK ( ! functionFound );
}

BOOST_AUTO_TEST_CASE ( loadIntVoidFunc) {
	BOOST_TEST_MESSAGE ( "TEST: loadIntVoidFunc");
	BOOST_TEST_MESSAGE ( "Load a function that returns an int and does not take any parameters" );

	std::string function_name ( "TestCategory.Int_Void_Func" );
	bool functionFound = (*LookupHelper::f_map).find (function_name) != (*LookupHelper::f_map).end();

	BOOST_CHECK ( functionFound == true );
	if ( functionFound ) {
		std::pair < ROOT::Reflex::Member, ExcelFunction > & value_pair = (*LookupHelper::f_map)[ function_name ];
		ROOT::Reflex::Member & member = value_pair.first;
		ExcelFunction & ef = value_pair.second;

		BOOST_CHECK ( ef.getName ( ) == "Int_Void_Func") ;
		BOOST_CHECK ( ef.getCategory ( ) == "TestCategory" ) ;
		BOOST_CHECK ( member.IsDataMember ( ) == false ) ;
		BOOST_CHECK ( member.IsFunctionMember ( ) == true ) ;
		BOOST_CHECK ( member.Name ( ) == "Int_Void_Func" ) ;
		BOOST_CHECK ( 0 == member.FunctionParameterSize ( false ) ) ;

		Reflex::Type type = member.TypeOf ( );
		Reflex::Type return_type = type.ReturnType ( );
		BOOST_CHECK ( "int" == return_type.Name ( ) );
	}
}

BOOST_AUTO_TEST_CASE ( loadIntIntFunc ) {
	BOOST_TEST_MESSAGE ( "TEST: loadIntIntFunc" );
	BOOST_TEST_MESSAGE ( "Load a function that returns an int and takes one int as a parameter" );

	std::string function_name ( "TestCategory.Int_Int_Func" );
	bool functionFound = (*LookupHelper::f_map).find ( function_name ) != (*LookupHelper::f_map).end();

	BOOST_CHECK ( functionFound == true );
	if ( functionFound ) {
		std::pair < ROOT::Reflex::Member, ExcelFunction > & value_pair = (*LookupHelper::f_map)[ function_name ];
		ROOT::Reflex::Member & member = value_pair.first;
		ExcelFunction & ef = value_pair.second;

		BOOST_CHECK ( ef.getName ( ) == "Int_Int_Func" ) ;
		//BOOST_TEST_MESSAGE ( "ef.getArgs() == " << ef.getArgs().size() );
		BOOST_CHECK ( ef.getArgs().size()  == 1 );
		BOOST_CHECK ( ef.getArgs().at(0) == "i_arg" );
		BOOST_CHECK ( ef.getCategory ( ) == "TestCategory" ) ;
		BOOST_CHECK ( member.IsDataMember ( ) == false ) ;
		BOOST_CHECK ( member.IsFunctionMember ( ) == true ) ;
		BOOST_CHECK ( member.Name ( ) == "Int_Int_Func" ) ;
		BOOST_CHECK ( 1 == member.FunctionParameterSize ( false ) ) ;

		Reflex::Type type = member.TypeOf ( );
		Reflex::Type return_type = type.ReturnType ( );
		BOOST_CHECK ( "int" == return_type.Name ( ) );

		Reflex::Type typeParm1 = type.FunctionParameterAt ( 0 );
		BOOST_CHECK ( "int" == typeParm1.Name ( ) );
	}
}

BOOST_AUTO_TEST_CASE ( loadStringStringFunc ) {
	BOOST_TEST_MESSAGE ( "TEST: loadStringStringFunc" );
	BOOST_TEST_MESSAGE ( "Load a function that returns a std::string and takes one std::string as a parameter" );

	std::string function_name ( "TestCategory.String_String_Func" );
	bool functionFound = (*LookupHelper::f_map).find ( function_name ) != (*LookupHelper::f_map).end();

	BOOST_CHECK ( functionFound == true );
	if ( functionFound ) {
		std::pair < ROOT::Reflex::Member, ExcelFunction > & value_pair = (*LookupHelper::f_map)[ function_name ];
		ROOT::Reflex::Member & member = value_pair.first;
		ExcelFunction & ef = value_pair.second;

		BOOST_CHECK ( ef.getName ( ) == "String_String_Func" ) ;
		BOOST_CHECK ( ef.getCategory ( ) == "TestCategory" ) ;
		BOOST_CHECK ( member.IsDataMember ( ) == false ) ;
		BOOST_CHECK ( member.IsFunctionMember ( ) == true ) ;
		BOOST_CHECK ( member.Name ( ) == "String_String_Func" ) ;
		BOOST_CHECK ( 1 == member.FunctionParameterSize ( false ) ) ;

		Reflex::Type type = member.TypeOf ( );
		Reflex::Type return_type = type.ReturnType ( );
		BOOST_CHECK ( "string" == return_type.Name ( ) );

		Reflex::Type typeParm1 = type.FunctionParameterAt ( 0 );
		BOOST_CHECK ( "string" == typeParm1.Name ( ) );
	}
}

BOOST_AUTO_TEST_CASE ( loadDoubleDoubleFunc ) {
	BOOST_TEST_MESSAGE ( "TEST: loadDoubleDoubleFunc" );
	BOOST_TEST_MESSAGE ( "Load a function that returns a double and takes one double as a parameter" );

	std::string function_name ( "TestCategory.Double_Double_Func" );
	bool functionFound = (*LookupHelper::f_map).find ( function_name ) != (*LookupHelper::f_map).end();

	BOOST_CHECK ( functionFound == true );
	if ( functionFound ) {
		std::pair < ROOT::Reflex::Member, ExcelFunction > & value_pair = (*LookupHelper::f_map)[ function_name ];
		ROOT::Reflex::Member & member = value_pair.first;
		ExcelFunction & ef = value_pair.second;

		BOOST_CHECK ( ef.getName ( ) == "Double_Double_Func" ) ;
		BOOST_CHECK ( ef.getCategory ( ) == "TestCategory" ) ;
		BOOST_CHECK ( member.IsDataMember ( ) == false ) ;
		BOOST_CHECK ( member.IsFunctionMember ( ) == true ) ;
		BOOST_CHECK ( member.Name ( ) == "Double_Double_Func" ) ;
		BOOST_CHECK ( 1 == member.FunctionParameterSize ( false ) ) ;

		Reflex::Type type = member.TypeOf ( );
		Reflex::Type return_type = type.ReturnType ( );
		BOOST_CHECK ( "double" == return_type.Name ( ) );

		Reflex::Type typeParm1 = type.FunctionParameterAt ( 0 );
		BOOST_CHECK ( "double" == typeParm1.Name ( ) );
	}
}
BOOST_AUTO_TEST_SUITE_END ( )
