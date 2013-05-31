package bijoux.bijoux.xlserver.server;

import static org.junit.Assert.*;

import org.junit.BeforeClass;
import org.junit.Test;

import bijoux.xlserver.excel.xloper.XLArray;
import bijoux.xlserver.excel.xloper.XLInt;
import bijoux.xlserver.excel.xloper.XLNum;
import bijoux.xlserver.excel.xloper.XLString;
import bijoux.xlserver.excel.xloper.XLoper;
import bijoux.xlserver.functions.General;
import bijoux.xlserver.functions.MyMath;
import bijoux.xlserver.server.FunctionReferencer;

public class FunctionReferencerTest {

	private static FunctionReferencer functionReferencer = null;

	@BeforeClass
	public static void beforeSetUp() {
		functionReferencer = new FunctionReferencer();
		functionReferencer.addMethodsFromClass ( TestClassOfFunctions.class );
		functionReferencer.addMethodsFromClass ( MyMath.class );
		functionReferencer.addMethodsFromClass ( General.class );
	}

	@Test
	public void testAddAllMethodsFromPricingClass ( ) { 
		assertTrue ( functionReferencer.doesMethodExist ( "TestClassOfFunctions.getString" ) );
	}

	@Test
	public void testCallMethodWithReturnValueOnly ( ) {
		assertEquals ( new XLString("Hello World"), functionReferencer.callMethod ( "TestClassOfFunctions.getString", null ) );
	}

	@Test
	public void testCallMethodWithTwoIntParametersA ( ) throws Exception {
		assertTrue ( functionReferencer.doesMethodExist ( "TestClassOfFunctions.addTwoIntegers" ) );
		assertEquals ( new XLInt(4), functionReferencer.callMethod ( "TestClassOfFunctions.addTwoIntegers", new XLoper[] { new XLInt(1), new XLInt(3) } ) );
	}

	@Test
	public void testCallMethodWithTwoIntParametersB ( ) throws Exception {
		assertTrue ( functionReferencer.doesMethodExist ( "TestClassOfFunctions.addTwoIntegers" ) );
		assertEquals ( new XLInt(3), functionReferencer.callMethod ( "TestClassOfFunctions.addTwoIntegers", new XLoper[] { new XLInt(0), new XLInt(3) } ) );
	}

	@Test
	public void callMethodTwoFLoatParametersA ( ) throws Exception {
		assertTrue ( functionReferencer.doesMethodExist ( "TestClassOfFunctions.addTwoFloats" ) );
		assertEquals ( new XLNum ( 3f ), functionReferencer.callMethod ( "TestClassOfFunctions.addTwoFloats", new XLoper[] { new XLNum(0f), new XLNum(3f) } ) );
	}

	@Test
	public void callMethodTwoFLoatParametersB ( ) throws Exception {
		assertTrue ( functionReferencer.doesMethodExist ( "TestClassOfFunctions.addTwoFloats" ) );
		assertEquals ( new XLNum ( 3.250f ), functionReferencer.callMethod ( "TestClassOfFunctions.addTwoFloats", new XLoper[] { new XLNum(0.250f), new XLNum(3f) } ) );
	}

	@Test
	public void callMethodTwoStringParameters ( ) {
		assertTrue ( functionReferencer.doesMethodExist ( "TestClassOfFunctions.getString" ) );
		assertEquals ( new XLString("Hello World"), 
				new XLString (
						TestClassOfFunctions.getString() 
					) );
	}

	@Test
	public void methodReturnsXLInt ( ) {
		assertTrue ( functionReferencer.doesMethodExist ( "TestClassOfFunctions.getInt" ) );
		assertEquals ( new XLInt( 100 ),
				new XLInt (
						TestClassOfFunctions.getInt ( ) 
				) ); 
	}

	@Test
	public void methodReturnsXLNum ( ) {
		assertTrue ( functionReferencer.doesMethodExist ( "TestClassOfFunctions.getNum" ) );
		assertEquals ( new XLNum( 0.1234d ),
				new XLNum (
						TestClassOfFunctions.getNum ( ) 
				));
	}

	@Test
	public void callMethodInGenericWay ( ) {
		assertTrue ( functionReferencer.doesMethodExist ( "TestClassOfFunctions.catTwoStrings" ) );
		assertEquals ( new XLString ( "JohnStratoudakis" ),
				functionReferencer.callMethod ( "TestClassOfFunctions.catTwoStrings", new XLoper[] { new XLString ( "John" ), new XLString ( "Stratoudakis" ) } ) );
	}

	@Test
	public void getInfoForOneFunction ( ) {
		assertTrue ( functionReferencer.doesMethodExist ( "MyMath.addTwoNumbers" ) );
		XLoper res = functionReferencer.getInfoForFunction ( "MyMath.addTwoNumbers" );
		assertEquals ( XLoper.xlTypeMulti, res.type );
		XLArray xlArr = (XLArray)res;
		assertEquals ( 5, xlArr.rows );
		assertEquals ( 2, xlArr.columns );
		assertEquals ( XLoper.xlTypeStr, xlArr.array[0].type );
	}
}