
#include "FunctionCaller.h"
#include "FunctionLoader.h"
#include "ExcelFunction.h"

//#include <boost/foreach.hpp>

#include <log4cxx/logger.h>
#include <log4cxx/xml/domconfigurator.h>

using namespace log4cxx;
using namespace log4cxx::xml;
using namespace log4cxx::helpers;

//func_map * FunctionLoader::all_functions = NULL;

LoggerPtr loggerFunctionCaller ( Logger::getLogger( "FunctionCaller" ) );
//typedef boost::unordered_map< std::string, std::pair<ROOT::Reflex::Member, ExcelFunction > > func_map;

bool FunctionCaller::CallFunction (
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
				//BOOST_TEST_MESSAGE ( "[short int] Adding parameter: " << parameters.at(i)->val.w << ", as: " << *p_i_parm ) ;
				parmList.push_back ( si_parm ) ;
			} else if ( type_of_parm.Name().compare("double") == 0 ) {
				double *p_d_parm = &parameters.at(i)->val.num;
				void *d_parm = p_d_parm;
				parmList.push_back ( d_parm ) ;
//				BOOST_TEST_MESSAGE ( "[double] Adding parameter: " << std::setprecision(15) << parameters.at(i)->val.num );
			} else if ( type_of_parm.Name().compare("string") == 0 ) {
				std::string *p_s_parm = new std::string();
				(p_s_parm)->append ( &parameters.at(i)->val.str[1], parameters.at(i)->val.str[0]);
//				BOOST_TEST_MESSAGE ( "[std::string] Adding parameter: " << *p_s_parm );
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
//			BOOST_TEST_MESSAGE ( "PARM RETURNED:" << std::setprecision(15) << d_result  );
			retVal->xltype = xltypeNum;
			retVal->val.num = d_result;
		} else if ( return_type.Name().compare("string") == 0 ) {
			std::string return_val;
			member.Invoke ( return_val, parmList );
			XLUtil::MakeString ( retVal, return_val.c_str() );
		}

		// [5] - Clean up memory of parameters
//		for ( int i = 0; i < parmList.size(); i++ ) {
//			delete parmList[i];
//		}

		return true;
	} else {
		retVal->xltype = xltypeErr;
		retVal->val.err = 1;
		return true;
	}

	return false;
}
