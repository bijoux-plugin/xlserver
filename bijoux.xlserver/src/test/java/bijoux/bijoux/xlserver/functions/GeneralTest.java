package bijoux.bijoux.xlserver.functions;

import static org.junit.Assert.assertEquals;

import org.junit.Test;

import bijoux.xlserver.BijouxServer;
import bijoux.xlserver.excel.xloper.XLoper;

public class GeneralTest {

	@Test
	public void testGetAllFunctionsNull ( ) {
		XLoper xlResult = BijouxServer.functionReferencer.callMethod ( "org.boris.xlloop.GetFunctions",  null );
		assertEquals ( XLoper.xlTypeMulti, xlResult.type );
	}

	@Test
	public void testGetAllFunctionsEmptyParm ( ) {
		XLoper xlResult = BijouxServer.functionReferencer.callMethod ( "org.boris.xlloop.GetFunctions",  new XLoper[] { } );
		assertEquals ( XLoper.xlTypeMulti, xlResult.type );
	}
}