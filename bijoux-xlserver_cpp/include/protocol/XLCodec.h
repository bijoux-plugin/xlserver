
#pragma once

#include "XLCodecCommon.h"
#include "XStream.h"

class XLCodec {
public:

	static void encode ( const char* str, XStream& xs );
	static void encode ( bool b, XStream& xs );
	static void encode ( int w, XStream& xs );
	static void encode ( const LPXLOPER xl, XStream& xs );

	static void decode ( XStream& xs, LPXLOPER xl );
};
