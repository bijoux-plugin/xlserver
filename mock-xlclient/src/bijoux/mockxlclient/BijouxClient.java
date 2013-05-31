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
		BijouxClient bijouxClient = new BijouxClient ( "localhost", 6000, "stratou", "stratou-localhost", "1.0.0", "1.0");
	
		bijouxClient.connect();
	
		bijouxClient.getListOfFunctions();

		//callHeartbeat();
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

			dumpOutXlOper(BinaryCodec.decode(is)); // Message
			dumpOutXlOper(BinaryCodec.decode(is)); // Boolean TRUE

			connected = true;
		} catch (IOException e) {
			connected = false;
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
					System.out.println ( "Function Name: " + xlMap.getString( "functionName" ) );
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