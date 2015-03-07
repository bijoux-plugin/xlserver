
#include "protocol/XLCodec.h"

#include <stdlib.h>

inline void writeDoubleWord ( unsigned int value, XStream& xs ) {

	xs.put(value >> 24 & 0xff);
	xs.put(value >> 16 & 0xff);
	xs.put(value >> 8 & 0xff);
	xs.put(value & 0xff);
}

inline void writeDouble ( double value, XStream& xs ) {

	unsigned int* p = (unsigned int *)&value;
	writeDoubleWord ( p[1], xs );
	writeDoubleWord ( p[0], xs );
}

inline int readDoubleWord ( XStream& xs ) {
	return (xs.get() << 24) & 0xff000000 | (xs.get() << 16)  & 0x00ff0000 | (xs.get() << 8) & 0x0000ff00 | (xs.get() & 0x000000ff);
}

inline double readDouble ( XStream& xs ) {
	int v1 = readDoubleWord ( xs );
	int v2 = readDoubleWord ( xs );
	double val;
	unsigned int* p = (unsigned int*)&val;
	p[1] = v1;
	p[0] = v2;
	return val;
}

void XLCodec::encode ( const LPXLOPER xl, XStream& xs ) {

	int type = xl->xltype & ~(xlbitXLFree | xlbitDLLFree);
//	std::cout << "\nXLCodec::encode ( type = " << type << " )";

	UINT len;
	int t = 0;
	switch ( type ) {
		case xltypeBool:
			xs.put ( XL_CODEC_TYPE_BOOL );
			xs.put ( (char) xl->val.xbool );
			break;
		case xltypeErr:
			xs.put ( XL_CODEC_TYPE_ERR );
			writeDoubleWord ( xl->val.err, xs );
			break;
		case xltypeInt:
			xs.put ( XL_CODEC_TYPE_INT );
			writeDoubleWord ( xl->val.w, xs );
			break;
		case xltypeMulti:
//			std::cout << "\nxs.put ( " << XL_CODEC_TYPE_MULTI << ")";
			xs.put ( XL_CODEC_TYPE_MULTI );
//			std::cout << "\n\trows: " << xl->val.array.rows;
//			std::cout << "\n\tcolumns: " << xl->val.array.columns;
			writeDoubleWord ( xl->val.array.rows, xs );
			writeDoubleWord ( xl->val.array.columns, xs );
			len = xl->val.array.rows*xl->val.array.columns;
			for(UINT i = 0; i < len; i++) {
				encode ( &xl->val.array.lparray[i], xs );
			}
			break;
		case xltypeNum:
			xs.put ( XL_CODEC_TYPE_NUM );
			writeDouble ( xl->val.num, xs );
			break;
		case xltypeStr:
//			std::cout << "\nEncoding String";
//			std::cout << "\nvalue: ";
			xs.put ( XL_CODEC_TYPE_STR );
			xs.put ( xl->val.str[0] );
//			for ( int i=0; i < xl->val.str[0]; i++ ) {
//				std::cout << xl->val.str[1+i];
//			}
			xs.write ( &xl->val.str[1], (unsigned char) xl->val.str[0] );
			break;
		case xltypeNil:
			xs.put ( XL_CODEC_TYPE_NIL );
			break;
		case xltypeSRef:
			xs.put ( XL_CODEC_TYPE_SREF );
			writeDoubleWord ( xl->val.sref.ref.colFirst, xs );
			writeDoubleWord ( xl->val.sref.ref.colLast, xs );
			writeDoubleWord ( xl->val.sref.ref.rwFirst, xs );
			writeDoubleWord ( xl->val.sref.ref.rwLast, xs );
			break;
		case XL_CODEC_TYPE_BIGDATA:
			xs.put ( XL_CODEC_TYPE_BIGDATA );
			writeDoubleWord ( xl->val.bigdata.cbData, xs );
			xs.write ( (char*)xl->val.bigdata.h.lpbData, xl->val.bigdata.cbData );
			break;
		case XL_CODEC_TYPE_MISSING:
		default:
			xs.put ( XL_CODEC_TYPE_MISSING );
			break;
	}
}

void XLCodec::encode ( const char* str, XStream& xs ) {

	xs.put ( XL_CODEC_TYPE_STR );
	uint len = strlen(str);
	xs.put(len);
	xs.write(str, len);
}

void XLCodec::encode(bool b, XStream& xs) {

	xs.put ( XL_CODEC_TYPE_BOOL );
	xs.put ( b?1:0 );
}

void XLCodec::encode ( int w, XStream& xs ) {

	xs.put ( XL_CODEC_TYPE_INT );
	writeDoubleWord ( w, xs );
}

void XLCodec::decode ( XStream& xs, LPXLOPER xl ) {

	int type = xs.get ( );
	if ( type == -1 || !xs.valid() ) {
		std::cout << "Connection on longer valid" << std::endl;
	}

	std::cout << "\nXLCodec::decode - type = 0x" << std::hex << type << std::endl;

	UINT len;
	switch ( type ) {
		case XL_CODEC_TYPE_BOOL:
			xl->xltype = xltypeBool;
			xl->val.xbool = xs.get ( );
			break;
		case XL_CODEC_TYPE_ERR:
			xl->xltype = xltypeErr;
			xl->val.err = readDoubleWord ( xs );
			break;
		case XL_CODEC_TYPE_INT:
			xl->xltype = xltypeInt;
			xl->val.w = readDoubleWord ( xs );
			break;
		case XL_CODEC_TYPE_MULTI:
			xl->xltype = xltypeMulti;
			xl->val.array.rows = readDoubleWord ( xs );
			xl->val.array.columns = readDoubleWord ( xs );
			if ( xs.valid ( ) ) {
				len = xl->val.array.rows * xl->val.array.columns;
				xl->val.array.lparray = (LPXLOPER) malloc ( sizeof(XLOPER) * len );
				for(UINT i = 0; i < len; i++)
				{
					decode ( xs, &xl->val.array.lparray[i] );
				}
			}
			break;
		case XL_CODEC_TYPE_NUM:
			xl->xltype = xltypeNum;
			xl->val.num = readDouble ( xs );
			break;
		case XL_CODEC_TYPE_STR:
			xl->xltype = xltypeStr;
			len = (unsigned char) xs.get();
			if ( xs.valid ( ) ) {
				xl->val.str = new char[len+2];
				xl->val.str[0] = (char) len;
				xl->val.str[len+1] = '\0';
				if(len > 0)
					xs.read ( &xl->val.str[1], len );
			}
			break;
		case XL_CODEC_TYPE_MISSING:
			xl->xltype = xltypeMissing;
			xl->val.str = 0;
			break;
		case XL_CODEC_TYPE_NIL:
			xl->xltype = xltypeNil;
			xl->val.str = 0;
			break;
		case XL_CODEC_TYPE_BIGDATA:
			xl->xltype = xltypeBigData;
			xl->val.bigdata.cbData = readDoubleWord ( xs );
			xl->val.bigdata.h.lpbData = (BYTE*) malloc(xl->val.bigdata.cbData * sizeof(BYTE) );
			if (xl->val.bigdata.cbData > 0)
				xs.read ( (char*)xl->val.bigdata.h.lpbData, xl->val.bigdata.cbData );
			break;
		default:
			xl->xltype = xltypeErr;
			xl->val.err = xlerrNA;
			break;
	}

	// Check for valid socket
	if ( !xs.valid ( ) )
		xl->xltype = xltypeNil;

	xl->xltype |= xlbitDLLFree;
}
