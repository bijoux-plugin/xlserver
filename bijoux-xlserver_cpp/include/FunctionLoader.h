

#ifndef FUNCTION_LOADER_H_
#define FUNCTION_LOADER_H_

#include "ExcelFunction.h"
#include <Reflex/Reflex.h>

#include <string>
#include <utility>
#include <vector>
#include <boost/unordered_map.hpp>

typedef boost::unordered_map< std::string, std::pair<ROOT::Reflex::Member, ExcelFunction > > func_map;

class FunctionLoader {

public:
	static void loadFunctions ( std::string className, func_map& f_map );
	static void loadAllFunctions ( std::vector < std::string > allClasses );
	static void dumpAllFunctions ( );

	static func_map * getAllfunctionsPtr ( ) {
		return FunctionLoader::all_functions;
	}

private:
	static func_map * all_functions;
};

#endif
