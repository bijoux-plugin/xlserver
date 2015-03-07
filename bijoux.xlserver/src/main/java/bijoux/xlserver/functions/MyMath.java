package bijoux.xlserver.functions;

import bijoux.xlserver.server.XLFunction;

public class MyMath {

	@XLFunction(
			name = "addTwoNumbers",
			help = "Adds two numbers",
			args = { "a", "b" },
			argHelp = { "first number", "second number" },
			category = "MyMath",
			isVolatile = true
			)
	public static int addTwoNumbers ( int a, int b) {
		System.out.println ("addTwoNumbers was called: " + a + ", " + b );
		return a + b;
	}

	@XLFunction(
			name = "doubleANumber",
			help = "Doubles a numbers",
			args = { "a" },
			argHelp = { "first number" },
			category = "MyMath",
			isVolatile = true
			)
	public static int doubleANumber ( int a ) {
		System.out.println ("doubleANumber was called: " + a );
		return a + a;
	}
}
