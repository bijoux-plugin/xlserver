dnl Process this file by invoking autoreconf --verbose --install

AC_PREREQ(2.59)
AC_INIT([bijoux.XlServer], [1.0], [johnstratoudakis@gmail.com])

AM_INIT_AUTOMAKE([-Wall -Werror foreign])

LT_INIT([disable-shared])

AC_CANONICAL_HOST
AC_MSG_NOTICE([Detected "$host" as the host operating system])
AC_MSG_NOTICE([Host CPU: "$host_cpu"])
AC_MSG_NOTICE([Host Vendor: "$host_vendor"])
AC_MSG_NOTICE([Host OS: "$host_os"])

case $host_os in
	darwin* )
		AC_MSG_NOTICE([Building on OSX])
		AC_SUBST([EXTRA_CPPFLAGS], ['-arch x86_64 -DBOOST_HAS_INT128=1'])
		;;
	linux* )
		AC_MSG_NOTICE([Building on Linux])
		;;
	*)
		AC_MSG_NOTICE([Unknown OS])
		;;
esac

AC_PROG_CXX
AC_CONFIG_MACRO_DIR([m4])

dnl I like using config.h because it is an easy way to know if we are running 
dnl a debug build
AC_CONFIG_HEADERS([config.h])

AC_CONFIG_FILES(Makefile src/Makefile)

dnl Boost Libraries
dnl Threading, Unit Test Framework
AX_BOOST_BASE([1.49],,[AC_MSG_ERROR([bijoux.XlXserver needs Boost 1.49 or above.  It was not found in your system])])
AX_BOOST_SYSTEM
AX_BOOST_THREAD
AX_BOOST_UNIT_TEST_FRAMEWORK

AC_OUTPUT
