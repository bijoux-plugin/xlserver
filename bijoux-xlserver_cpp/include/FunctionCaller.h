

#ifndef FUNCTION_CALLER_H_
#define FUNCTION_CALLER_H_

#include "ExcelFunction.h"
#include "FunctionLoader.h"
#include <Reflex/Reflex.h>
#include "XLCall.h"

#include <string>
//#include <utility>
//#include <vector>
//#include <boost/unordered_map.hpp>
//
//typedef boost::unordered_map< std::string, std::pair<ROOT::Reflex::Member, ExcelFunction > > func_map;

/* FunctionCaller
 *
 * Purpose:
 * 		The Bijoux Server will receive a function call from the Excel client in the following format:
 * 			<function name>
 * 			number of parameters
 * 			array of XLOPER parameters
 * 		Our purpose is to facilitate the calling of functions in classes that are dynamically loaded
 * 		by the FunctionLoader class and 
 */

class FunctionCaller {

public:

	static bool CallFunction (
                    func_map *f_map,
                    std::string function_name,
					LPXLOPER retVal,
                    std::vector<LPXLOPER> parameters );
private:
};

#endif
