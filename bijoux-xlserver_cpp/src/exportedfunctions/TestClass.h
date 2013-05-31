/*
 * TestClass.h
 *
 * Class to be used for unit testing only, including tests for loading functions
 * and for calling functions.
 */

#ifndef TESTCLASS_H_
#define TESTCLASS_H_

#include "ExcelFunction.h"

class TestClass {
public:
	TestClass();
	virtual ~TestClass();

/*
  @custom:ExcelFunction (
		Name="Int_Void_Func",
		Help="Function that takes zero parameters and return an integer",
		Category="TestCategory" 
	)
 */
	static int Int_Void_Func ( void ) ;

/*
  @custom:ExcelFunction ( Name="Int_Int_Func",
		Args= { "i_arg" },
		Help="Function that takes one integer parameter and return an integer",
		Category="TestCategory" 
	)
*/
	static int Int_Int_Func ( int ) ;

/*
  @custom:ExcelFunction ( Name="Int_Int_Int_Func",
		Help="Function that takes two integer parameters and return an integer",
		Category="TestCategory"  )
*/
	static int Int_Int_Int_Func ( int, int ) ;

/*
  @custom:ExcelFunction ( Name="String_Void_Func",
		Help="Function that takes one integer parameter and return an integer",
		Category="TestCategory" 
	)
*/
	static std::string String_Void_Func ( void ) ;
/*
  @custom:ExcelFunction ( Name="String_String_Func",
		Help="Function that takes one integer parameter and return an integer",
		Category="TestCategory" 
	)
*/
	static std::string String_String_Func ( std::string ) ;

/*
  @custom:ExcelFunction ( Name="Double_Double_Func",
		Help="Function that takes one double parameter and returns a double",
		Category="TestCategory" 
	)
*/
	static double Double_Double_Func ( double ) ;
/*
  @custom:ExcelFunction ( Name="String_Double_Int_String_Func",
		Help="Function that takes one double parameter, one integer, one string and returns a string",
		Category="TestCategory" 
	)
*/
	static std::string String_Double_Int_String_Func ( double, int, std::string ) ;
};
#endif /* TESTCLASS_H_ */
