package bijoux.bijoux.xlserver.server;

import bijoux.xlserver.server.XLFunction;

public class TestClassOfFunctions {

	@XLFunction(
			name = "getString",
			help = "Return a string value",
			args = { },
			argHelp = { },
			category = "TestClassOfFunctions"
			)
	public static String getString ( ) {
		return "Hello World";
	}

	@XLFunction(
			name = "addTwoIntegers",
			help = "Adds two integers",
			args = { "x", "y"},
			argHelp = { "summand 1", "summand 2" },
			category = "TestClassOfFunctions"
			)
	public static int addTwoIntegers ( int x, int y ) {
		return x + y;
	}

	@XLFunction(
			name = "addTwoFloats",
			help = "Adds two floating point numbers",
			args = { "x", "y"},
			argHelp = { "summand 1", "summand 2" },
			category = "TestClassOfFunctions"
			)
	public static double addTwoFloats ( double x, double y ) {
		return x + y;
	}

	@XLFunction(
			name = "getInt",
			help = "Return an integer value",
			args = { },
			argHelp = { },
			category = "TestClassOfFunctions"
			)
	public static int getInt ( ) {
		return 100;
	}

	@XLFunction(
			name = "getNum",
			help = "Return a double (Num) value",
			args = { },
			argHelp = { },
			category = "TestClassOfFunctions"
			)
	public static double getNum() {
		return 0.1234d;
	}

	@XLFunction(
			name = "catTwoStrings",
			help = "concatenates two strings",
			args = { "a", "b"},
			argHelp = { "first string", "second string" },
			category = "TestClassOfFunctions"
			)
	public static String catTwoStrings ( String a, String b) {
		return a + b;
	}
}