
#define BOOST_TEST_DYN_LINK

#define LOG4CXX_INFO(logger, expr)

#include <boost/test/unit_test.hpp>
#include <boost/foreach.hpp>

#include <iomanip>

#include "protocol/XLUtil.h"
#include "FunctionLoader.h"
#include "FunctionCaller.h"
#include "XLCall.h"

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

BOOST_FIXTURE_TEST_SUITE ( FunctionCallerTests, LookupHelper )

bool CallFunction (
		func_map *f_map,
		std::string function_name,
		LPXLOPER retVal,
		std::vector<LPXLOPER> parameters ) {

	bool functionFound = (*f_map).find ( function_name ) != (*f_map).end();

	if ( functionFound ) {
		std::pair < ROOT::Reflex::Member, ExcelFunction > & value_pair = (*f_map)[ function_name ];
		ROOT::Reflex::Member & member = value_pair.first;

		// [2] - Prepare function parameters
		Reflex::Type type = member.TypeOf ( );
		std::vector<void *> parmList;

		for ( int i = 0; i < member.FunctionParameterSize ( ) ; i++ ) {
			Reflex::Type type_of_parm = type.FunctionParameterAt ( i );
			if ( type_of_parm.Name().compare("int") == 0 ) {
				int *p_i_parm = new int;
				*p_i_parm = (int)parameters.at(i)->val.w;
				void *si_parm = p_i_parm;
				BOOST_TEST_MESSAGE ( "[short int] Adding parameter: " << parameters.at(i)->val.w << ", as: " << *p_i_parm ) ;
				parmList.push_back ( si_parm ) ;
			} else if ( type_of_parm.Name().compare("double") == 0 ) {
				double *p_d_parm = &parameters.at(i)->val.num;
				void *d_parm = p_d_parm;
				parmList.push_back ( d_parm ) ;
				BOOST_TEST_MESSAGE ( "[double] Adding parameter: " << std::setprecision(15) << parameters.at(i)->val.num );
			} else if ( type_of_parm.Name().compare("string") == 0 ) {
				std::string *p_s_parm = new std::string();
				(p_s_parm)->append ( &parameters.at(i)->val.str[1], parameters.at(i)->val.str[0]);
				BOOST_TEST_MESSAGE ( "[std::string] Adding parameter: " << *p_s_parm );
				parmList.push_back ( (void*) p_s_parm );
			}
		}

		void * p_result = NULL;
		Reflex::Type return_type = type.ReturnType ( );
		if ( return_type.Name ( ).compare ( "int" ) == 0 ) {
			p_result = (void *) new int;
			member.Invoke ( p_result, parmList );
			int p_i_result = (__int64_t) (p_result) & 0xFF;
			retVal->xltype = xltypeInt;
			retVal->val.w = p_i_result;
		} else if ( return_type.Name().compare("double") == 0 ) {
			double d_result;
			member.Invoke ( d_result, parmList );
			BOOST_TEST_MESSAGE ( "PARM RETURNED:" << std::setprecision(15) << d_result  );
			retVal->xltype = xltypeNum;
			retVal->val.num = d_result;
		} else if ( return_type.Name().compare("string") == 0 ) {
			std::string return_val;
			member.Invoke ( return_val, parmList );
			XLUtil::MakeString ( retVal, return_val.c_str() );
		}

		// [5] - Clean up memory of parameters
		for ( int i = 0; i < parmList.size(); i++ ) {
			delete parmList[i];
		}

		return true;
	} else {
		retVal->xltype = xltypeErr;
		retVal->val.err = 1;
		return true;
	}

	return false;
}

BOOST_AUTO_TEST_CASE ( callIntVoidFunc ) {
	BOOST_TEST_MESSAGE ( "TEST: callIntVoidFunc" );
	BOOST_TEST_MESSAGE ( "Testing the behavior of calling a function that returns an int and takes no parameters." );

	std::string function_name = "TestCategory.Int_Void_Func";
	bool functionFound = (*LookupHelper::f_map).find ( function_name ) != (*LookupHelper::f_map).end ( ) ;
	BOOST_CHECK ( functionFound == true ) ;

	// Prepare source parameters
	std::vector<LPXLOPER> parameters;

	// Prepare return value
	LPXLOPER retVal = new XLOPER;

	// Call function
	bool result = FunctionCaller::CallFunction ( LookupHelper::f_map, function_name, retVal, parameters ) ;

	BOOST_CHECK ( true == result ) ;
	BOOST_CHECK ( retVal->xltype == xltypeInt ) ;
	BOOST_CHECK ( retVal->val.w == 0xDC ) ;
}

BOOST_AUTO_TEST_CASE ( callIntIntFunc ) {
	BOOST_TEST_MESSAGE ( "TEST: callIntIntFunc" );
	BOOST_TEST_MESSAGE ( "Testing the behavior of calling a function that returns an int and an int as a parameter." );

	std::string function_name = "TestCategory.Int_Int_Func";

	bool functionFound = (*LookupHelper::f_map).find ( function_name ) != (*LookupHelper::f_map).end();
	BOOST_CHECK ( functionFound == true );

	// Prepare source parameters
	std::vector<LPXLOPER> parameters;
	LPXLOPER lpParm1 = XLUtil::MakeInteger ( 123 ) ;
	parameters.push_back ( lpParm1 ) ;

	// Prepare return value
	LPXLOPER retVal = new XLOPER;
	bool result = CallFunction ( LookupHelper::f_map, function_name, retVal, parameters ) ;

	BOOST_CHECK ( true == result ) ;
	BOOST_CHECK ( retVal->xltype == xltypeInt ) ;
	BOOST_CHECK ( retVal->val.w == 246 ) ;
}

BOOST_AUTO_TEST_CASE ( callIntIntIntFunc ) {
	BOOST_TEST_MESSAGE ( "TEST: callIntIntIntFunc" );
	BOOST_TEST_MESSAGE ( "Testing the behavior of calling a function that returns an int and an int as a parameter." );

	std::string function_name = "TestCategory.Int_Int_Int_Func";

	bool functionFound = (*LookupHelper::f_map).find ( function_name ) != (*LookupHelper::f_map).end();
	BOOST_CHECK ( functionFound == true );

	// Prepare source parameters
	std::vector<LPXLOPER> parameters;
	LPXLOPER lpParm1 = XLUtil::MakeInteger ( 123 ) ;
	parameters.push_back ( lpParm1 ) ;
	LPXLOPER lpParm2 = XLUtil::MakeInteger ( 100 ) ;
	parameters.push_back ( lpParm2 ) ;

	// Prepare return value
	LPXLOPER retVal = new XLOPER;
	bool result = FunctionCaller::CallFunction ( LookupHelper::f_map, function_name, retVal, parameters ) ;

	BOOST_CHECK ( result == true ) ;
	BOOST_CHECK ( retVal->xltype == xltypeInt ) ;
	BOOST_CHECK ( retVal->val.w == 223 ) ;
}

BOOST_AUTO_TEST_CASE ( callStringVoidFunc ) {
	BOOST_TEST_MESSAGE ( "TEST: callStringVoidFunc" );
	BOOST_TEST_MESSAGE ( "Testing the behavior of calling a function that returns an std::string and takes no parameters." );

	std::string function_name = "TestCategory.String_Void_Func";

	// Prepare source parameters
	std::vector<LPXLOPER> parameters;

	// Prepare return value
	LPXLOPER retVal = new XLOPER;
	bool result = FunctionCaller::CallFunction ( LookupHelper::f_map, function_name, retVal, parameters ) ;

	BOOST_CHECK ( result == true ) ;
	BOOST_CHECK ( retVal->xltype == xltypeStr ) ;
	BOOST_CHECK ( XLUtil::getXlOperValue ( retVal ) == "Hello World!" ) ;
}

BOOST_AUTO_TEST_CASE ( callStringStringFunc ) {
	BOOST_TEST_MESSAGE ( "TEST: callStringStringFunc" );
	BOOST_TEST_MESSAGE ( "Testing the behavior of calling a function that returns an std::string and takes one std::string parameter." );

	std::string function_name = "TestCategory.String_String_Func";

	// Prepare source parameters
	std::vector<LPXLOPER> parameters;
	LPXLOPER lpParm1 = new XLOPER;
	XLUtil::MakeString ( lpParm1, "test" ) ;
	parameters.push_back ( lpParm1 ) ;

	// Prepare return value
	LPXLOPER retVal = new XLOPER;
	bool result = FunctionCaller::CallFunction ( LookupHelper::f_map, function_name, retVal, parameters ) ;

	BOOST_CHECK ( result == true ) ;
	BOOST_CHECK ( retVal->xltype == xltypeStr ) ;
	BOOST_CHECK ( XLUtil::getXlOperValue ( retVal ) == "test.oneStringParamFunc()" ) ;
}

BOOST_AUTO_TEST_CASE ( callDoubleDoubleFunc ) {
	BOOST_TEST_MESSAGE ( "TEST: callDoubleDoubleFunc" );
	BOOST_TEST_MESSAGE ( "Testing the behavior of calling a function that returns a double and takes one double parameter." );

	std::string function_name = "TestCategory.Double_Double_Func";

	// Prepare source parameters
	std::vector<LPXLOPER> parameters;
	LPXLOPER lpParm1 = new XLOPER;
	XLUtil::MakeDouble ( lpParm1, 123.123L ) ;
	parameters.push_back ( lpParm1 ) ;

	// Prepare return value
	LPXLOPER retVal = new XLOPER;
	bool result = FunctionCaller::CallFunction ( LookupHelper::f_map, function_name, retVal, parameters ) ;

	BOOST_CHECK ( result == true ) ;
	BOOST_CHECK ( retVal->xltype == xltypeNum ) ;
	BOOST_CHECK_CLOSE ( retVal->val.num, 246.246L, 0.000001 ) ;
}

BOOST_AUTO_TEST_CASE ( callStringDoubleIntStringFunc ) {
	BOOST_TEST_MESSAGE ( "TEST: callStringDoubleIntStringFunc" );
	BOOST_TEST_MESSAGE ( "Testing the behavior of calling a function that returns a stringnd takes one double, one int, and one std::string parameter." );

	std::string function_name = "TestCategory.String_Double_Int_String_Func";

	// Prepare source parameters
	std::vector<LPXLOPER> parameters;
	LPXLOPER lpParm1 = new XLOPER;
	XLUtil::MakeDouble ( lpParm1, 123.123L ) ;
	parameters.push_back ( lpParm1 ) ;

	LPXLOPER lpParm2 = new XLOPER;
	XLUtil::MakeInteger ( lpParm2, 1999 ) ;
	parameters.push_back ( lpParm2 ) ;

	LPXLOPER lpParm3 = new XLOPER;
	XLUtil::MakeString ( lpParm3, "DXD-SDS" ) ;
	parameters.push_back ( lpParm3 ) ;

	// Prepare return value
	LPXLOPER retVal = new XLOPER;
	bool result = FunctionCaller::CallFunction ( LookupHelper::f_map, function_name, retVal, parameters ) ;

	BOOST_CHECK ( result == true ) ;
	BOOST_CHECK ( retVal->xltype == xltypeStr ) ;
	BOOST_CHECK ( "123.12,1999,DXD-SDS" == XLUtil::getXlOperValue ( retVal ) );
}


BOOST_AUTO_TEST_SUITE_END ( )
