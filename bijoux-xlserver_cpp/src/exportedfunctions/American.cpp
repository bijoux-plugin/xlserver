/*
 * ClassOfFunctions.cpp
 *
 *  Created on: May 24, 2013
 *      Author: stratj2
 */

#include "American.h"

American::American() {
}

American::~American() {
}

int American::priceAmericanOption ( int strikePrice ) {

	strikePrice += 10;

//	LPXLOPER lpReturnInt = new XLOPER;
//	XLUtil::MakeNumber ( lpReturnInt, strikePrice + 10 );

	//int return_value = 89;
//	LPXLOPER lpReturnInt = XLUtil::MakeNumber ( strikePrice + 10 );
//	lpReturnInt->val.num = strikePrice;

	//XLUtil::MakeNumber ( lpReturnInt, strikePrice );

	return ( strikePrice );
}

