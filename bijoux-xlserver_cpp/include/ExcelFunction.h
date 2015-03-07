
#ifndef _EXCEL_FUNCTION_H
#define _EXCEL_FUNCTION_H

#include "XLCall.h"
#include "protocol/XLUtil.h"

#include <ostream>
#include <string>
#include <vector>

class ExcelFunction {

public:
	ExcelFunction () { }

	ExcelFunction ( const ExcelFunction &other ) : name ( other.name ), help ( other.help ),
						args ( other.args ), argHelp ( other.argHelp ),
						category ( other.category ), isVolatile ( other.isVolatile ) {
	}

	friend std::ostream& operator<<(std::ostream &strm, const ExcelFunction &a) {
		strm << "ExcelFuncion.toString() =  " << a.name << ", " << a.help << ", {";
		for ( int i = 0; i < a.args.size(); i++ ) {
			if ( i == a.args.size() - 1 )
 				strm << "\"" << a.args.at(i) << "\"";
			else
 				strm << "\"" << a.args.at(i) << "\", ";
		}
 		strm << "}";
		return strm;
	}

	std::string getName ( ) const { return name; }
	void setName ( const std::string & value ) { name = value; }

	std::string getCategory ( ) const { return category; }
	void setCategory ( const std::string & value ) { category = value; }

	std::string getHelp ( ) const { return help; }
	void setHelp ( const std::string & value ) { help = value; }

	bool getIsVolatile ( ) const { return isVolatile; }
	void setIsVolatile ( const bool value ) { isVolatile = value; }

	std::vector<std::string> &getArgs ( ) { return args; }
	void setArgs ( const std::vector<std::string> value ) { args = value; }

	std::vector<std::string> &getArgHelp ( ) { return argHelp; }
	void setArgHelp ( const std::vector<std::string> value ) { argHelp = value; }


private:
        std::string name;
        std::string category;
        std::string help;
		bool isVolatile;
		std::vector<std::string> args;
        std::vector<std::string> argHelp;
};

#endif
