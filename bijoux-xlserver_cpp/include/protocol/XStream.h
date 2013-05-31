/*
 * XStream.h
 * Wrapper class that uses boost::asio
 */

#ifndef XSTREAM_H_
#define XSTREAM_H_

#define STREAM_BUF_SIZE 4096

#include <iostream>

#include <boost/asio.hpp>
using boost::asio::ip::tcp;

class XStream {

public:
	XStream ( tcp::socket& socket ) :
					socket_ ( socket ),
					out_pos ( 0 ), out_len ( 0 ),
					in_pos ( 0 ), in_len ( 0 ) {
	}

	int get ( );
	void read ( char* s, unsigned int n );

	void put ( char c );
	void write ( const char* s, unsigned n );
	void flush ( );

	inline bool valid ( ) {
		return socket_.is_open();
	}

private:
	void fill ( );

	char in_buffer_ [ STREAM_BUF_SIZE ];
	int in_pos;
	int in_len;

	char out_buffer_ [ STREAM_BUF_SIZE ];
	int out_pos;
	int out_len;

	tcp::socket& socket_;
};

inline void XStream::fill ( ) {

	if ( false == socket_.is_open ( ) ) {
		return;
	}

	size_t r = socket_.read_some ( boost::asio::buffer ( in_buffer_, STREAM_BUF_SIZE ) );

	if ( r == 0 )
		return;

	if ( r == -1) {
		in_len = 0;
		return;
	}
	if ( r > 0 ) {
		in_len = r;
	}
	in_pos = 0;
}

inline int XStream::get ( ) {

	if ( false == socket_.is_open ( ) ) {
		return -1;
	}
	if ( in_pos >= in_len ) {
		fill ( );
	}
	return in_buffer_ [ in_pos++ ] & 0xffff;
}

inline void XStream::put ( char c ) {
	if ( out_pos >= STREAM_BUF_SIZE )
		flush ( );
	out_buffer_ [ out_pos++ ] = c;
}

inline void XStream::read ( char* s, unsigned int n ) {

	if ( s == 0 || n <= 0 )
		return;

	if ( in_pos >= in_len )
		fill ( );

	int i = 0;
	while ( n > 0 && s ) {
		unsigned int size = in_len - in_pos;
		if ( size > n )
			size = n;
		if ( size > 0 ) {
			memcpy ( &s[i], &in_buffer_ [ in_pos ], size );
			in_pos += size;
		}
		n -= size;
		i += size;
		if ( n > 0 && in_pos >= in_len )
			fill(); // pos=0, len=0..STREAM_BUF_SIZE
	}
}

inline void XStream::flush ( ) {

	if ( false == valid ( ) )
		return;

	if ( out_pos > 0 ) {
		int len = 0;
		int i = 0;
		while ( valid ( ) && i < out_pos ) {

			// Makes a direct call to the Socket _sock
			len = socket_.send ( boost::asio::buffer ( &out_buffer_[i], out_pos - i ) );
			//len = send(_sock, &buf[i], pos-i, 0); // never sure how much is sent

			if ( len == -1 ) {
				socket_.close ( );
				//close ( _sock );
				//_sock = 0;
			}
			i += len;
		}
		out_pos = 0;
	}
}

inline void XStream::write ( const char* s, unsigned int n ) {

	if ( n <= 0 ) return;
	if ( n > STREAM_BUF_SIZE ) {
		unsigned int i = 0;
		while ( i < n && s ) {
			unsigned int size = STREAM_BUF_SIZE - out_pos;
			if ( size > n )
				size = n;
			memcpy ( &out_buffer_[ out_pos ], &s[i], size );
			i += size;
			if ( out_pos >= STREAM_BUF_SIZE )
				flush ( );
		}
	} else {
		if ( out_pos + n >= STREAM_BUF_SIZE )
			flush();
		memcpy(&out_buffer_[ out_pos ], s, n);
		out_pos += n;
	}
}

#endif /* XSTREAM_H_ */
