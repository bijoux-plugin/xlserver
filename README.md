
+----------------------------------------------------+
|  To build the C++ version of the Bijoux XL Server  |
+----------------------------------------------------+

+----------------------------------------+
|  OS X -  Darwin Kernel Version 13.0.0  |
|  Mavericks                             |
+----------------------------------------+

I have run these instructions on multiple Virtual Machines running OS X Mavericks as well
as on multiple actual Apple machines running this version of the kernel.

You need to have the latest version of XCode as well as the Command Line Tools for XCode
installed for this to work.  I chose to use Homebrew for installing many of the other
required components.  You can easily use other such tools such as MacPorts, Fink.  You can
probably even build those tools yourself.

1 - Install or make sure XCode has been installed on your machine
	https://developer.apple.com/downloads

2 - Install Command Line Tools for XCode
	https://developer.apple.com/downloads

3 - Install Homebrew

	ruby -e "$(curl -fsSL https://raw.github.com/Homebrew/homebrew/go/install)"
	brew doctor
	brew update
	brew tap homebrew/versions

	# The following command will take some time and use lots of CPU power
	brew install autoconf automake libtool boost cmake log4cxx

4 - Install GccXML

	git clone git://github.com/gccxml/gccxml.git
	cd gccxml && mkdir gccxml-build && cd gccxml-build
	CC=gcc-4.4 CXX=g++-4.4 cmake ..
	make && sudo make install

5 - Install Reflex

	git clone git://github.com/GiannisRambo/Reflex.git
	cd Reflex
	build/autogen
	./configure && make && sudo make install

6 - Install Bijoux

	git clone git@github.com:bijoux-plugin/xlserver.git
	cd xlservers
	cd bijoux-xlserver_cpp
	./build.sh
	./run-xlserver.sh

+----------------------------------------+
|  OS X -  Darwin Kernel Version 12.5.0  |
|  Mountain Lion                         |
+----------------------------------------+

Here is what I did on a Virtual Machine, I will try to replicate this
on an actual OS X box later on tonight when I get home

1 - Install or make sure XCode has been installed on your machine
	https://developer.apple.com/downloads

2 - Install Command Line Tools for XCode
	https://developer.apple.com/downloads

3 - Install Homebrew

	ruby -e "$(curl -fsSL https://raw.github.com/Homebrew/homebrew/go/install)"

	*** If you get Certificate Errors...etc, run the following and try the above
	    command again

	echo insecure >> ~/.curlrc
	echo --no-epsv >> ~/.curlrc
	git config --global http.sslVerify false

	modify Ruby open-uri.rb to ignore VERIFICATION
	sudo /System/Library/Frameworks/Ruby.framework/Versions/1.8/usr/lib/ruby/1.8/open-uri.rb
	http.verify_mode = OpenSSL::SSL::VERIFY_NONE
	#http.verify_mode = OpenSSL::SSL::VERIFY_PEER

	brew doctor
	brew update
	brew tap homebrew/versions
	# The following command will take some time and use lots of CPU power
	brew install autoconf automake libtool boost gcc44 cmake

4 - Install GccXML

	git clone git://github.com/gccxml/gccxml.git
	cd gccxml && mkdir gccxml-build && cd gccxml-build
	CC=gcc-4.4 CXX=g++-4.4 cmake ..
	make && sudo make install

5 - Install Reflex

	git clone git://github.com/GiannisRambo/Reflex.git
	cd Reflex
	# patch build/autogen to use glibtoolize instead of libtoolize
	build/autogen
	./configure CC=gcc-4.8 CXX=g++-4.8 && make && sudo make install

6 - Install Log4cxx

	brew install log4cxx

	Again, if you get errors with downloading, do the following to pick a
	mirror

	brew edit log4cxx and modify url to: (or any other mirror)
		http://mirror.sdunix.com/apache/logging/log4cxx/0.10.0/apache-log4cxx-0.10.0.tar.gz

7 - Install Bijoux

	git clone git@github.com:bijoux-plugin/xlserver.git
	cd xlservers
	git checkout --track origin/add_cpp_version (<= my syntax may be wrong)
	cd bijoux-xlserver_cpp
	autoreconf -fvi && ./configure
	make check
	make
	./run-xlserver.sh

+-----------------------------------+
|  Linux - Fedora Core 20 (64-bit)  |
+-----------------------------------+

My machine has the following kernel:

	[john@localhost ~]$ uname -a

+----------------+
|  Dependencies  |
+----------------+

Install any missing dependencies or tools via the following yum commands:

	sudo yum install boost-devel log4cxx-devel

To build you need to install the Reflex library: (I am working on a custom RPM)

	git clone git://github.com/GiannisRambo/Reflex.git
	cd Reflex
	build/autogen
	./configure --enable-minimal
	make
	sudo make install
	echo "export LD_LIBRARY_PATH=/usr/local/lib:\$LD_LIBRARY_PATH" >> ~/.bash_profile
	source ~/.bash_profile

+----------------------------------+
|  Build Instructions:             |
+----------------------------------+
cd bijoux-xlserver_cpp
autoreconf -fvi && ./configure && make check
./run-xlserver.sh
# Now run the client


