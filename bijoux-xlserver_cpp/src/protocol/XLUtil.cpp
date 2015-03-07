/*
 * XLUtil.cpp
 *
 *  Created on: Jan 28, 2014
 *      Author: stratj2
 */

/*******************************************************************************
* This program and the accompanying materials
* are made available under the terms of the Common Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/cpl-v10.html
*
* Contributors:
*     Peter Smith
*******************************************************************************/

#include <cstdlib>
#include <cstring>
#include <stdarg.h>

#include <sstream>

#include "protocol/XLUtil.h"
#include "XLCall.h"

#define TRUE 1
#define FALSE 0

//void XLUtil::MakeNumber(LPXLOPER lpXLOper, const int number) {
//	lpXLOper->xltype = xltypeNum;
//	lpXLOper->val.num = number;
//}

void XLUtil::MakeInteger(LPXLOPER lpXLOper, const int number) {
	lpXLOper->xltype = xltypeInt;
	lpXLOper->val.w = number & 0xFFFF;
}
void XLUtil::MakeDouble(LPXLOPER lpXLOper, const double number) {
	lpXLOper->xltype = xltypeNum;
	lpXLOper->val.num = number;
}
void XLUtil::MakeString(LPXLOPER lpXLOper, const char* string)
{
	lpXLOper->xltype = xltypeStr;
	lpXLOper->val.str = XLUtil::MakeExcelString(string);
}

void XLUtil::MakeMissing(LPXLOPER lpXLOper)
{
	lpXLOper->xltype = xltypeNil;
}
void XLUtil::MakeBoolean(LPXLOPER lpXLOper, const bool boolean_value)
{
	lpXLOper->xltype = xltypeBool;
	if ( boolean_value )
		lpXLOper->val.xbool = TRUE;
	else
		lpXLOper->val.xbool = FALSE;
}

LPXLOPER XLUtil::MakeBoolean(const bool boolean_value)
{
	LPXLOPER xl = new XLOPER;
	xl->xltype = xltypeBool;
	if ( boolean_value )
		xl->val.xbool = TRUE;
	else
		xl->val.xbool = FALSE;

	return xl;
}
LPXLOPER XLUtil::MakeMissing()
{
	LPXLOPER xl = new XLOPER;
	xl->xltype = xltypeMissing;
	return xl;
}
//LPXLOPER XLUtil::MakeNumber(const int number)
//{
//	LPXLOPER xl = new XLOPER;
//	xl->xltype = xltypeNum;
//	xl->val.num = number;
//	return xl;
//}
LPXLOPER XLUtil::MakeInteger(const int number)
{
	LPXLOPER xl = new XLOPER;
	xl->xltype = xltypeInt;
	xl->val.w = number & 0xFFFF;
	return xl;
}
// Make an excel string
LPSTR XLUtil::MakeExcelString(const char* string)
{
	if(string == NULL) return NULL;
	size_t len = strlen(string);
	if(len > 255) len = 255; // Excel strings are limited to 255 chars
	char* temp = (char *) malloc(len + 2);
	memcpy(temp + 1, string, len);
	temp[0] = (BYTE) len;
	temp[len+1] = 0;
	return temp;
}

LPXLOPER XLUtil::MakeExcelString2(const char* string)
{
	LPXLOPER xl = new XLOPER;
	if(string == NULL) {
		xl->xltype = xltypeMissing;
	} else {
		xl->xltype = xltypeStr;
		xl->val.str = MakeExcelString(string);
	}

	return xl;
}

LPXLOPER XLUtil::MakeExcelString3(char* lcstr)
{
	LPXLOPER xl = new XLOPER;
	if(lcstr == NULL) {
		xl->xltype = xltypeMissing;
	} else {
		xl->xltype = xltypeStr;
		xl->val.str = lcstr;
	}
	return xl;
}

void XLUtil::CopyValue(LPXLOPER xloperSrc, LPXLOPER xloperDst)
{
	memcpy(xloperDst, xloperSrc, sizeof(XLOPER));
	xloperDst->xltype = (xloperSrc->xltype & ~(xlbitXLFree | xlbitDLLFree));
}

void XLUtil::FreeContents(LPXLOPER px)
{
	switch(px->xltype & ~(xlbitXLFree | xlbitDLLFree))
	{
		case xltypeMulti:
			for ( int i = (px->val.array.rows) * (px->val.array.columns) - 1;
							i >=0; i--)
			{
				FreeContents ( &px->val.array.lparray[i] );
			}
			free(px->val.array.lparray);
			break;
		case xltypeStr:
			if ( px != NULL && px->val.str != NULL )
			{
				free(px->val.str);
				px->val.str = NULL;
			}
			break;
		case xltypeBigData:
			if ( px != NULL && px->val.bigdata.h.lpbData != NULL )
			{
				free(px->val.bigdata.h.lpbData);
				px->val.bigdata.h.lpbData = NULL;
			}
			break;
		default:
			break;
	}
}

void XLUtil::MakeDLLFree(LPXLOPER px)
{
	if ( px != NULL )
		px->xltype |= xlbitDLLFree;
}

void XLUtil::MakeXLFree(LPXLOPER px)
{
	if ( px != NULL )
		px->xltype |= xlbitXLFree;
}

std::string XLUtil::getXlOperType ( LPXLOPER lpXlOper ) {

	std::stringstream out_string;
	DWORD myType = lpXlOper->xltype & 0x0FFF;
	switch ( myType ) {
		case ( XL_CODEC_TYPE_NUM ) :
			return std::string ( "Double" );
			break;
		case ( XL_CODEC_TYPE_STR ):
			return std::string ( "String" );
			break;
		case ( XL_CODEC_TYPE_BOOL ):
			return std::string ( "Boolean" );
			break;
//		case ( XL_CODEC_TYPE_SREF ):
//			return std::string ( "Reference" );
//			break;
		case ( xltypeErr ):
			out_string << "Error: " << myType ;
			break;
		case ( xltypeFlow ):
			return std::string ( "Flow" );
			break;
		case ( xltypeMulti ):
			return std::string ( "Multi (Array)" );
			break;
		case ( xltypeMissing ):
			return std::string ( "Missing" );
			break;
		case ( xltypeNil ):
			return std::string ( "Nil" );
			break;
		case ( xltypeSRef ):
			return std::string ( "SRef" );
			break;
		case ( xltypeInt ):
			return std::string ( "Integer" );
			break;
		case ( xltypeBigData ):
			out_string << "BigData";
			break;
		default:
			out_string << "Unknown: " << myType;
			break;
	}
	return out_string.str();
}

std::string XLUtil::getXlOperValue ( LPXLOPER xlOper ) {

	std::stringstream out_string;
	int len = 0;
	WORD myType = xlOper->xltype & 0x0FFF;
	//std::cout << "\ngetXlOperValue - type = " << myType;

	switch ( myType ) {
		case XL_CODEC_TYPE_BOOL:
			if ( xlOper->val.xbool == 1 ) {
				out_string << "True";
			} else {
				out_string << "False";
			}
			break;
		case XL_CODEC_TYPE_NUM:
			out_string << xlOper->val.num;
			break;
		case XL_CODEC_TYPE_STR:
			len = (unsigned char)xlOper->val.str[0];
			for ( int i=0; i < len; ++i ) {
				out_string << xlOper->val.str[i + 1];
			}
			break;
		case xltypeInt:
			out_string << xlOper->val.w;
			break;
/*
		case xltypeErr:
			xl->xltype = xltypeErr;
			xl->val.err = readDoubleWord(is);
			break;
		case xltypeMulti:
			xl->xltype = xltypeMulti;
			xl->val.array.rows = readDoubleWord(is);
			xl->val.array.columns = readDoubleWord(is);
			if(is.valid())
			{
				len = xl->val.array.rows * xl->val.array.columns;
				xl->val.array.lparray = (LPXLOPER) malloc(sizeof(XLOPER) * len);
				for(UINT i = 0; i < len; i++)
				{
					decode(is, &xl->val.array.lparray[i]);
				}
			}
			break;
		case xltypeMissing:
			xl->xltype = xltypeMissing;
			xl->val.str = 0;
			break;
		case xltypeNil:
			xl->xltype = xltypeNil;
			xl->val.str = 0;
			break;
		case xltypeBigData:
			xl->xltype = xltypeBigData;
			xl->val.bigdata.cbData = readDoubleWord(is);
			xl->val.bigdata.h.lpbData = (BYTE*)malloc(xl->val.bigdata.cbData * sizeof(BYTE) );
			if (xl->val.bigdata.cbData > 0)
				is.read ( (char*)xl->val.bigdata.h.lpbData, xl->val.bigdata.cbData );
			break;
		*/
		default:
			out_string << "[Unknown]";
			break;
	}
	return ( out_string.str ( ) );
}

// Assumes a two-column array with key/value on each row
LPXLOPER XLMap::get(LPXLOPER pmap, const char* key)
{
	if(key == NULL) return NULL;
	int len = strlen(key);
	int rows = pmap->val.array.rows;
	if(rows == 0) return NULL;
	int cols = pmap->val.array.columns;
	if(cols != 2) return NULL;
	int cells = rows * cols;
	for(int i =0; i < cells; i += 2)
	{
		LPXLOPER k = &pmap->val.array.lparray[i];
		if((k->xltype & ~(xlbitXLFree | xlbitDLLFree)) != xltypeStr)
			continue;
		int l = k->val.str[0];
		if(len != l)
			continue;
		if(strncmp(key, &k->val.str[1], l) == 0)
			return &pmap->val.array.lparray[i+1];
	}
	return NULL;
}

char* XLMap::getString(LPXLOPER pmap, const char* key)
{
	LPXLOPER px = get(pmap, key);
	if(px != NULL && (px->xltype & ~(xlbitXLFree | xlbitDLLFree)) == xltypeStr) {
		return px->val.str;
	}
	return NULL;
}

bool XLMap::getBoolean(LPXLOPER pmap, const char* key)
{
	LPXLOPER px = get(pmap, key);
	if(px != NULL && (px->xltype & ~(xlbitXLFree | xlbitDLLFree)) == xltypeBool)
	{
		return (px->val.xbool == 1 );
	}
	return false;
}

int XLMap::getInteger(LPXLOPER pmap, const char* key)
{
	LPXLOPER px = get(pmap, key);
	if(px != NULL && (px->xltype & ~(xlbitXLFree | xlbitDLLFree)) == xltypeInt) {
		return px->val.w;
	}
	return -1;
}
