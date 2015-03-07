/*
 * General.h
 *
 *  Created on: May 24, 2013
 *      Author: stratj2
 */

#ifndef GENERAL_H_
#define GENERAL_H_

#include "ExcelFunction.h"
#include <vector>
#include <string>

class General {
public:
	General();
	virtual ~General();

/*
  @custom:ExcelFunction(
        Name="GetFunctions",
        Category="org.boris.xlloop",
        Help="Get Number One argument Help"
        )
 */
	static std::vector<std::string> GetFunctions ( void );

};
#endif /* GENERAL_H_ */
