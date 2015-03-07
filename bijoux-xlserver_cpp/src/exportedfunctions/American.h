/*
 * American.h
 *
 *  Created on: Febryary 5, 2014
 *      Author: John Stratoudakis
 */

#ifndef AMERICAN_H_
#define AMERICAN_H_ 

#include "ExcelFunction.h"

class American {
public:
	American ();
	virtual ~American ();

/*
  @custom:ExcelFunction(
        Name="priceAmericanOption",
        Help="Function to Price an American Option",
		Args = { "a", "b" },
		ArgHelp = { "how to use a", "how to use b" },
        Category="American",
        Help="Price American Option argument Help",
		IsVolatile=false
        )
 */
	static int priceAmericanOption ( int strikePrice );

};

#endif /* AMERICAN_H_ */
