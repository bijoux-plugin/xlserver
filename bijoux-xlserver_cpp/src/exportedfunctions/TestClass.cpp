
#include "TestClass.h"
#include "protocol/XLUtil.h"

#include <sstream>
#include <iomanip>

TestClass::TestClass () {
}

TestClass::~TestClass () {
}

int TestClass::Int_Void_Func ( void ) {
	return 0xFEDC;
}

int TestClass::Int_Int_Func ( int parm1 ) {
	return parm1 + parm1 ;
}

int TestClass::Int_Int_Int_Func ( int parm1, int parm2 ) {
	return parm1 + parm2 ;
}
std::string TestClass::String_Void_Func ( void ) {
	return "Hello World!";
}
std::string TestClass::String_String_Func ( std::string strParm ) {
	return strParm.append ( ".oneStringParamFunc()" ) ;
}
double TestClass::Double_Double_Func ( double parm1 ) {
	return parm1 + parm1;
}
std::string TestClass::String_Double_Int_String_Func ( double parm1, int parm2, std::string parm3 ) {
	std::stringstream results;
	results << std::setprecision(5) << parm1 << "," << parm2 << "," << parm3;
	return results.str();
}
