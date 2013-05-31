package bijoux.xlserver.server;

import java.lang.reflect.Method;

public class FunctionInformation {

	private XLFunction xf = null;
	private Method method = null;

	public FunctionInformation ( XLFunction xf, Method method ) {
		this.xf = xf;
		this.method = method;
	}

	public Method getMethod() {
		return this.method;
	}

	public XLFunction getXLFunction ( ) {
		return this.xf;
	}
}