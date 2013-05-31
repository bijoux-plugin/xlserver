package bijoux.mockxlclient;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;

import bijoux.mockxlclient.codec.BinaryCodec;
import bijoux.mockxlclient.xloper.XLArray;
import bijoux.mockxlclient.xloper.XLBool;
import bijoux.mockxlclient.xloper.XLInt;
import bijoux.mockxlclient.xloper.XLMap;
import bijoux.mockxlclient.xloper.XLString;
import bijoux.mockxlclient.xloper.XLoper;

public class BijouxClient {

	private boolean connected = false;

	private String targetHost;
	private int targetPort;
	private String userName;
	private String localMachineName;
	private String versionString;
	private String protocolVersion;

	private Socket socket;
	private OutputStream os;
	private InputStream is;

	public static void main(String[] args) {
		int port = 6000;
		BijouxClient bijouxClient = new BijouxClient ( "localhost", port, "stratou", "stratou-localhost", "1.0.0", "1.1" );

		System.out.println ( "Attempting to connect on port: " + port );
		bijouxClient.connect();

		System.out.println ( "Sleeping for 1 second..." );
		try {
			Thread.sleep ( 1000 );
		} catch ( InterruptedException ie ) {
		}
		bijouxClient.getListOfFunctions();

		System.out.println ( "Calling American.priceAmericanOption ( )" );
		bijouxClient.callPriceAmericanOption();

//		try {
//			Thread.sleep ( 1000 ) ;
//			bijouxClient.getListOfFunctions( ) ;
//		} catch ( InterruptedException ex ) {
//			System.out.println ( "InterruptedException: " + ex ) ;
//		}

		System.out.println ( "Disconnecting from server..." );
		bijouxClient.disconnect();
	}
	
	public BijouxClient(String targetHost, int targetPort, String userName,
			String localMachineName, String versionString, String protocolVersion) {
		this.targetHost = targetHost;
		this.targetPort = targetPort;
		this.userName = userName;
		this.localMachineName = localMachineName;
		this.versionString = versionString;
		this.protocolVersion = protocolVersion;
	}

	public void connect() {
		try {
			socket = new Socket(targetHost, targetPort);
			os = socket.getOutputStream();
			is = socket.getInputStream();

			BinaryCodec.encode(new XLString(userName), os);
			BinaryCodec.encode(new XLString(localMachineName), os);
			BinaryCodec.encode(new XLString(versionString), os);
			BinaryCodec.encode(new XLString(protocolVersion), os);
			os.flush();

			BinaryCodec.decode(is); // Logon acceptance
			BinaryCodec.decode(is); // Boolean TRUE

			connected = true;
		} catch (IOException e) {
			connected = false;
			System.out.flush();
			e.printStackTrace();
		}
	}

	public void callPriceAmericanOption() {
		try {
			BinaryCodec.encode(new XLString("American.priceAmericanOption"), os);
			BinaryCodec.encode(new XLInt(1), os);
			BinaryCodec.encode(new XLInt(100), os);
			os.flush();

			// Get Response from Function Call
			XLoper xloper = BinaryCodec.decode(is);
			//System.out.println ( "Response of function: " + xloper.type );
			BijouxClient.dumpOutXlOper ( xloper );
		} catch (IOException e) {
			connected = false;
			System.out.flush();
			e.printStackTrace();
		}
	}

	public void disconnect() {
		try {
			socket.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		connected = false;
	}

	public void getListOfFunctions() {
		try {
			System.out.println("Calling: org.boris.xlloop.GetFunctions");
			BinaryCodec.encode(new XLString("org.boris.xlloop.GetFunctions"), os);
			BinaryCodec.encode(new XLInt(0), os);
			os.flush();

			XLoper xloper = BinaryCodec.decode(is);

			if ( null != xloper ) {
				XLArray xlArray = ((XLArray)xloper);
				for ( int i = 0; i < xlArray.rows; i++ ) {
					XLArray funcInfo = ((XLArray)xlArray.get (i, 0) );
					XLMap xlMap = new XLMap ( funcInfo );
					System.out.println ( "======================================================================" );
					System.out.println ( "Function Name:\t\t" + xlMap.getString( "functionName" ) );
					System.out.println ( "\tCategory:\t\t" + xlMap.getString( "category" ) );
					System.out.println ( "\tFunction Help:\t\t" + xlMap.getString( "functionHelp" ) );
					System.out.println ( "\tIs Volatile:\t\t" + xlMap.getString("isVolatile") );
					System.out.println ( "\tArgument Text:\t\t" + xlMap.getString("argumentText") );
					XLArray argHelp = xlMap.getArray("argumentHelp");
					System.out.println ( "\tArgument Help:\t\t" + argHelp );
				}
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public static void dumpOutXlOper(XLoper xloper) {
		switch (xloper.type) {
			case XLoper.xlTypeInt:
				XLInt xlInt = (XLInt)xloper;
				System.out.println("[Int] = " + xlInt);
				break;
			case XLoper.xlTypeStr:
				XLString xlStr = (XLString)xloper;
				System.out.println("[String] = " + xlStr);
				break;
			case XLoper.xlTypeBool:
				System.out.println("[Boolean] = " + (XLBool)xloper);
				break;
			case XLoper.xlTypeMulti:
				XLArray xlArray = (XLArray)xloper;
				System.out.println("[Multi]");
				System.out.println("Rows = " + xlArray.rows);
				System.out.println("Columns = " + xlArray.columns);
				for (int r = 0; r < xlArray.rows; r++) {
					for (int c = 0; c < xlArray.columns; c++) {
						XLoper xllOper = xlArray.array[ (r * xlArray.columns) + c];
						dumpOutXlOper(xllOper);
					}
				}
				break;
			default:
				System.out.println ("[Other Type, implement me: " + xloper.type);
				break;
		}
		
	}

	public boolean isConnected() {
		return connected;
	}
}
