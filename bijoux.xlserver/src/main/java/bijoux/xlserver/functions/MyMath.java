package bijoux.xlserver.functions;

import bijoux.xlserver.server.XLFunction;

public class MyMath {

	@XLFunction(
			name = "addTwoNumbers",
			help = "Adds two numbers",
			args = { "a", "b" },
			argHelp = { "first number", "second number" },
			category = "MyMath"
			)
	public static int addTwoNumbers ( int a, int b) {
		System.out.println ("addTwoNumbers was called: " + a + ", " + b );
		return a + b;
	}
}