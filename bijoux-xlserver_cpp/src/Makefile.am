AUTOMAKE_OPTIONS = subdir-objects
AM_CPPFLAGS=$(BOOST_CPPFLAGS) $(EXTRA_CPPFLAGS) -fpermissive -Wall -I$(top_srcdir)/include -I/usr/local/include
AM_LDFLAGS=$(BOOST_LDFLAGS) -L/usr/local/lib

# Tests
# If OSX use -arch x86_64 -DBOOST_HAS_INT128=1 or -arch i386 
noinst_PROGRAMS=Tests
TESTS=Tests
LOG_DRIVER=./MyTestRunner.py

Tests_SOURCES=\
			FunctionLoader.cpp \
			FunctionLoaderTests.cpp \
			FunctionCaller.cpp \
			FunctionCallerTests.cpp \
			SocketListener.cpp \
			exportedfunctions/TestClass.cpp \
			exportedfunctions/TestClass_rflx.cpp \
			exportedfunctions/General.cpp \
			exportedfunctions/General_rflx.cpp \
			exportedfunctions/American.cpp \
			exportedfunctions/American_rflx.cpp \
			socketServer/TcpServer.cpp \
			socketServer/TcpConnection.cpp \
			protocol/XStream.cpp \
			protocol/XLUtil.cpp \
			protocol/XLCodec.cpp
#Tests_CPPFLAGS=$(EXTRA_CPPFLAGS) -fpermissive -I$(top_srcdir)/include
Tests_LDADD= \
			-llog4cxx -lReflex -lpthread \
			$(BOOST_UNIT_TEST_FRAMEWORK_LIB) \
			$(BOOST_THREAD_LIB) \
			$(BOOST_SYSTEM_LIB) \
			$(BOOST_LOG_LIB)

# Regular
bin_PROGRAMS = bijoux_xlserver

bijoux_xlserver_SOURCES=\
			server_main.cpp \
			SocketListener.cpp \
			FunctionLoader.cpp \
			FunctionCaller.cpp \
			exportedfunctions/General.cpp \
			exportedfunctions/General_rflx.cpp \
			exportedfunctions/American.cpp \
			exportedfunctions/American_rflx.cpp \
			socketServer/TcpServer.cpp \
			socketServer/TcpConnection.cpp \
			protocol/XStream.cpp \
			protocol/XLUtil.cpp \
			protocol/XLCodec.cpp
#bijoux_xlserver_CPPFLAGS=$(EXTRA_CPPFLAGS) -I$(top_srcdir)/include -Wall -Wno-unused-function -DBOOST_LOG_DYN_LINK
bijoux_xlserver_LDADD = \
			$(BOOST_THREAD_LIB) \
			$(BOOST_SYSTEM_LIB) \
			$(BOOST_LOG_LIB) \
			-lReflex -lpthread -llog4cxx

BUILT_SOURCES=exportedfunctions/General_rflx.cpp exportedfunctions/American_rflx.cpp exportedfunctions/TestClass_rflx.cpp
CLEANFILES=exportedfunctions/General_rflx.cpp exportedfunctions/American_rflx.cpp exportedfunctions/TestClass_rflx.cpp


exportedfunctions/TestClass_rflx.cpp: exportedfunctions/TestClass.h
	genreflex exportedfunctions/TestClass.h -I../include -I. -o exportedfunctions/TestClass_rflx.cpp

exportedfunctions/General_rflx.cpp: exportedfunctions/General.h
	genreflex exportedfunctions/General.h -I../include -I. -o exportedfunctions/General_rflx.cpp

exportedfunctions/American_rflx.cpp: exportedfunctions/American.h
	genreflex exportedfunctions/American.h -I../include -I. -o exportedfunctions/American_rflx.cpp

