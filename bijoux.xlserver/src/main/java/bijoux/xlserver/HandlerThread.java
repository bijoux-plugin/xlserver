package bijoux.xlserver;

import java.io.IOException;
import java.net.Socket;
import java.util.ArrayList;

import bijoux.xlserver.excel.codec.BinaryRequestProtocol;
import bijoux.xlserver.excel.xloper.XLBool;
import bijoux.xlserver.excel.xloper.XLInt;
import bijoux.xlserver.excel.xloper.XLString;
import bijoux.xlserver.excel.xloper.XLoper;

public class HandlerThread extends Thread {

	private BinaryRequestProtocol protocol = null;

	private Socket socket = null;

	private String userName = null;
	private String hostName = null;
	private String clientVersion = null;
	private String protocolVersion = null;

    public HandlerThread ( Socket socket) {
        super("Function Server Connection Handler Thread");
        this.socket = socket;
    }

    public void run() {

    	handleUserLogon();

    	while (!socket.isClosed()) {
    		try {
				handleFunctionCall ( );
			} catch (IOException e) {
				break;
			}
        }
    }

	private void handleFunctionCall() throws IOException {

		XLoper functionName = protocol.receive ( socket );
		XLoper numberOfParameters = protocol.receive ( socket );

		String func = ((XLString) functionName).str;
		int numOfParameters = ((XLInt) numberOfParameters).w;

		ArrayList<XLoper> parameters = new ArrayList<XLoper> ();

		for ( int i=0; i < numOfParameters; i++ ) {
			parameters.add ( protocol.receive( socket ) );
		}

		XLoper xlResult = BijouxServer.functionReferencer.callMethod( func, (XLoper[]) parameters.toArray (new XLoper[]{} ) );
		protocol.send ( socket, xlResult );
	}

	private void handleUserLogon() {
		protocol = new BinaryRequestProtocol();
		try {

			protocol.initialize(socket);
			XLoper userName = protocol.receive(socket);
			XLoper hostName = protocol.receive(socket);
			XLoper clientVersion = protocol.receive(socket);
			XLoper protocolVersion = protocol.receive(socket);

			this.userName = ((XLString) userName).str;
			this.hostName = ((XLString) hostName).str;
			this.clientVersion = ((XLString) clientVersion).str;
			this.protocolVersion = ((XLString) protocolVersion).str;

			protocol.send ( socket,
						new XLString("Received: " + getUserInfo ( )  + ". Connected successfully to " +
								java.net.InetAddress.getLocalHost().getHostName() + "."));
			protocol.send ( socket, new XLBool ( true ) );
		} catch (IOException e) {
		}
	}

    private String getUserInfo ( ) {
		return this.userName + ":" + this.hostName + ":" + this.clientVersion + ":" + this.protocolVersion;
	}

	public void close() {
        try {
            socket.getInputStream().close();
            socket.getOutputStream().close();
            socket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}