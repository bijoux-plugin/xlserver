package bijoux.xlserver;

import java.util.Scanner;

import bijoux.xlserver.server.FunctionReferencer;

public class BijouxServer {

	private int port = 6000;
	public static FunctionReferencer functionReferencer = new FunctionReferencer ( );

	public static void main ( String[] args ) {

		BijouxServer bijouxServer = new BijouxServer ( 6000 );
		bijouxServer.start();

		Scanner scanner = new Scanner(System.in);
		do {
			System.out.println ("Press q/Q to quit the server");
		} while ( false == scanner.next().equalsIgnoreCase("Q") ) ;
		scanner.close();
	}

	public BijouxServer ( int port ) {
		this.port = port;
	}

	public void start() {
		FunctionServer functionServer = new FunctionServer ( this.port );
		functionServer.start();		
	}
}