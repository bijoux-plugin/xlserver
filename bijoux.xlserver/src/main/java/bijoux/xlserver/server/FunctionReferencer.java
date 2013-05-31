package bijoux.xlserver.server;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.util.HashMap;
import bijoux.xlserver.excel.xloper.XLArray;
import bijoux.xlserver.excel.xloper.XLError;
import bijoux.xlserver.excel.xloper.XLList;
import bijoux.xlserver.excel.xloper.XLMap;
import bijoux.xlserver.excel.xloper.XLoper;
import bijoux.xlserver.functions.General;
import bijoux.xlserver.functions.MyMath;

public class FunctionReferencer {

	private HashMap<String, FunctionInformation> myFunctions = new HashMap<String,FunctionInformation>();
	private XLoperObjectConverter xlConv = new XLoperObjectConverter();

	public FunctionReferencer ( ) {
		addMethodsFromClass ( MyMath.class );
		addMethodsFromClass ( General.class );
	}

	public void addMethodsFromClass ( Class<?> clazz ) {
		Method [] methods = clazz.getMethods();
 		for ( Method method : methods ) {
			if ( Modifier.isStatic ( method.getModifiers() ) ) {
				XLFunction xf = method.getAnnotation (XLFunction.class);
				if ( null != xf ) {
					saveXLFunctionAndMethodReference ( xf, method );	
				}				
			}
		}
	}

	private void saveXLFunctionAndMethodReference(XLFunction xf, Method method) {
		String methodName = xf.category() + "." + xf.name();
		System.out.println ( "Adding method: " + methodName );
		myFunctions.put ( methodName, new FunctionInformation ( xf, method ) );
	}

	public boolean doesMethodExist ( String methodName ) {
		return myFunctions.containsKey ( methodName );
	}

	public XLoper callMethod ( String methodName, XLoper[] objectsToBeConverted ) {

		Method method = myFunctions.get ( methodName ).getMethod();
		Class<?>[] paramTypes = method.getParameterTypes();

		XLoper result = new XLError ( 0 );
		Object [] convertedObjects = new Object[] {};

		if ( null != objectsToBeConverted && objectsToBeConverted.length > 0 ) {
			convertedObjects = new Object [objectsToBeConverted.length];
			for ( int i=0; i < objectsToBeConverted.length; i++ ) {
				convertedObjects[i] = xlConv.createFrom ( objectsToBeConverted[i], paramTypes[i] );
			}	
		}

		try {
			Object objResult = method.invoke ( null, ( Object [ ] ) convertedObjects );
			result = xlConv.createFrom ( objResult );
		} catch (IllegalAccessException | IllegalArgumentException
				| InvocationTargetException e) {
			e.printStackTrace();
		}
		return result;
	}


	public XLArray getAllFunctions() {
		int i = 0;
		XLArray xlFunctions = new XLArray( myFunctions.keySet().size() , 1);
		for ( String functionName : myFunctions.keySet() ) {
			xlFunctions.array[i++] = getInfoForFunction ( functionName );
		}
		return xlFunctions;
	}

	public XLoper getInfoForFunction ( String functionName ) {
		FunctionInformation fi = myFunctions.get ( functionName );
		if ( null == fi ) {
			return new XLArray ( 0, 0 ); 
		}
		XLoper result = encode ( fi.getXLFunction ( ) );
		return result;
	}

	public XLoper encode ( XLFunction xlFunction ) {

		XLMap s = new XLMap ( );
		s.add("functionName", xlFunction.category() + "." + xlFunction.name() );
		if ( xlFunction.help() != null)
			s.add("functionHelp", xlFunction.help() );
		if ( xlFunction.category() != null)
			s.add("category", xlFunction.category() );
//		if ( shortcutText != null)
//			s.add("shortcutText", shortcutText);
//		if ( helpTopic != null)
//			s.add("helpTopic", helpTopic);
		if ( xlFunction.args().length > 0) {
			s.add("argumentText", toCSV ( xlFunction.args() ) );
			XLList c = new XLList();
			for (int i = 0; i < xlFunction.argHelp().length ; i++) {
				c.add((String) xlFunction.argHelp()[i]);
			}
			s.add("argumentHelp", c);
		}
		s.add("isVolatile", xlFunction.isVolatile() );
		return s.toXloper();
	}

    public static String toCSV(String[] arr) {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < arr.length; i++) {
            sb.append(arr[i]);
            if (i < arr.length - 1)
                sb.append(",");
        }
        return sb.toString();
    }
}
