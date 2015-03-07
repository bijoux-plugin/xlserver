/*
 * XLUtil.h
 *
 *  Created on: Jan 28, 2014
 *      Author: stratj2
 */

#ifndef XLUTIL_H_
#define XLUTIL_H_


/*******************************************************************************
 * This program and the accompanying materials
 * are made available under the terms of the Common Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/cpl-v10.html
 *
 * Contributors:
 *     Peter Smith
 *******************************************************************************/

#include <string>
#include "XLCodecCommon.h"
#include "XLCall.h"

class XLUtil {

public:
	static LPSTR MakeExcelString(const char* string);
	static LPXLOPER MakeExcelString2(const char* string);
	static LPXLOPER MakeExcelString3(char* lcstr);

//	static void MakeNumber(LPXLOPER lpXLOper, const int number);
	static void MakeDouble(LPXLOPER lpXLOper, const double number);
	static void MakeInteger(LPXLOPER lpXLOper, const int number);
	static void MakeString(LPXLOPER lpXLOper, const char* string);
	static void MakeMissing(LPXLOPER lpXLOper);
	static void MakeBoolean(LPXLOPER lpXLOper, const bool boolean_value);

//	static LPXLOPER MakeNumber(const int number);
	static LPXLOPER MakeInteger(const int number);
	static LPXLOPER MakeMissing();
	static LPXLOPER MakeBoolean(const bool boolean_value);

	static void CopyValue(LPXLOPER xloperSrc, LPXLOPER xloperDst);
	static void FreeContents(LPXLOPER px);
	static void MakeDLLFree(LPXLOPER px);
	static void MakeXLFree(LPXLOPER px);

	static std::string getXlOperType ( LPXLOPER lpXlOper );
	static std::string getXlOperValue ( LPXLOPER xlOper );
};

class XLMap {
public:
	static LPXLOPER get(LPXLOPER pmap, const char* key);
	static char* getString(LPXLOPER pmap, const char* key);
	static bool getBoolean(LPXLOPER pmap, const char* key);
	static int getInteger(LPXLOPER pmap, const char* key);
};


#endif /* XLUTIL_H_ */
