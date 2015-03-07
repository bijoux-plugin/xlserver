package bijoux.xlserver.functions;

import bijoux.xlserver.BijouxServer;
import bijoux.xlserver.excel.xloper.XLArray;
import bijoux.xlserver.excel.xloper.XLBool;
import bijoux.xlserver.server.XLFunction;

// I included this class so that it remains functionally compatible with the existing excel
// addin
public class General {

	@XLFunction(
			name = "GetFunctions",
			help = "Return a string value",
			args = { },
			argHelp = { },
			category = "org.boris.xlloop"
			)
	public static XLArray getListOfFunctions ( ) {
		return BijouxServer.functionReferencer.getAllFunctions();
	}

	@XLFunction(
			name = "heartbeat",
			help = "Return a string value",
			args = { },
			argHelp = { },
			category = "Calypso.Access"
			)
	public static XLBool Heartbeat ( ) {
		return new XLBool(true);
	}
}