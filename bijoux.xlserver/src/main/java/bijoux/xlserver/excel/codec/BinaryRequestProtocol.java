package bijoux.xlserver.excel.codec;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;
import java.net.Socket;

import bijoux.xlserver.excel.xloper.XLoper;

public class BinaryRequestProtocol {

	private BufferedInputStream  input;
	private BufferedOutputStream output;

	public void initialize(Socket socket) throws IOException {
		socket.setPerformancePreferences(0, 1, 0);
		input = new BufferedInputStream(socket.getInputStream());
		output = new BufferedOutputStream(socket.getOutputStream());
	}

	public XLoper receive(Socket socket) throws IOException {
		return BinaryCodec.decode(input);
	}

	public void send(Socket socket, XLoper data) throws IOException {
		BinaryCodec.encode(data, output);
		output.flush();
	}
}