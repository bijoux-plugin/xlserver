package bijoux.xlserver;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;


public class FunctionServer {

	protected int port;
	protected ServerSocket socket;

	public FunctionServer ( int port ) {
		this.port = port;
	}

	public void start ( ) {
		if (socket != null) {
			return;
		}

		Thread t = new Thread ( new Runnable ( ) {
			public void run ( ) {
				try {
					FunctionServer.this.run();
				} catch ( IOException e ) {
					e.printStackTrace ( );
				}
			}
		});

		t.setName ( "Function Server" );
		t.setDaemon ( true );
		t.start ( );
	}

	public void stop() throws IOException {
		if (socket != null) {
			socket.close();
		}
		socket = null;
	}

	public void run ( ) throws IOException {
		if ( socket == null ) {
			socket = new ServerSocket ( port );
		}

		while ( true ) {
			try {
				handleSocketInNewThread ( socket.accept ( ) );
			} catch ( ClassCastException e ) {
				System.out.println ( "ClassCastException" );
			} catch ( Exception e ) {
				System.out.println ( "Exception" );
			}
		}
	}

	private void handleSocketInNewThread ( Socket newSocket ) {
		HandlerThread ht = new HandlerThread ( newSocket  );
		ht.start ();
	}
}