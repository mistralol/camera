AC_INIT(camera, 1.0.0, james@stev.org)
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_SRCDIR([src/Camera.cpp])
AC_CONFIG_MACRO_DIR([m4])

LT_INIT
AM_INIT_AUTOMAKE([foreign])

AC_PROG_CC
AC_PROG_CXX

AX_CXX_COMPILE_STDCXX_11(noext, optional)

AX_COMPILER_FLAGS([WARN_CFLAGS],[WARN_LDFLAGS])
AC_SUBST([AM_CXXFLAGS], $WARN_CXXFLAGS)

AC_PROG_LIBTOOL

dnl give error and exit if we don't have pkgconfig
AC_CHECK_PROG(HAVE_PKGCONFIG, pkg-config, [ ], [
  AC_MSG_ERROR([You need to have pkg-config installed!])
])

dnl required versions of gstreamer and plugins-base
GST_REQUIRED=1.0.0
GSTPB_REQUIRED=1.0.0

PKG_CHECK_MODULES(GST, [
  gstreamer-1.0 >= $GST_REQUIRED
  gstreamer-base-1.0 >= $GST_REQUIRED
  gstreamer-controller-1.0 >= $GST_REQUIRED
  gstreamer-rtsp-server-1.0 >= $GST_REQUIRED
], [
  AC_SUBST(GST_CFLAGS)
  AC_SUBST(GST_LIBS)
], [
  AC_MSG_ERROR([
      You need to install or upgrade the GStreamer development
      packages on your system. On debian-based systems these are
      libgstreamer1.0-dev and libgstreamer-plugins-base1.0-dev.
      on RPM-based systems gstreamer1.0-devel, libgstreamer1.0-devel
      or similar. The minimum version required is $GST_REQUIRED.
  ])
])

dnl Check for jsoncpp
PKG_CHECK_MODULES(JSON, [
	jsoncpp >= 0.6.0
], [
	AC_SUBST(JSON_CFLAGS)
	AC_SUBST(JSON_LIBS)
], [
	AC_MSG_ERROR([
		You need to install or upgrade jsoncpp
	])
])

dnl Check for liblogger
PKG_CHECK_MODULES(LIBLOGGER, [
	liblogger >= 1.0.0
], [
	AC_SUBST(LIBLOGGER_CFLGAGS)
	AC_SUBST(LIBLOGGER_LIBS)
], [
	AC_MSG_ERROR([
		You need to install or upgrade liblogger
	])
])

dnl Check for libclientserver
PKG_CHECK_MODULES(LIBCLIENTSERVER, [
	libclientserver >= 1.0.0
], [
	AC_SUBST(LIBCLIENTSERVER_CFLGAGS)
	AC_SUBST(LIBCLIENTSERVER_LIBS)
], [
	AC_MSG_ERROR([
		You need to install or upgrade libclientserver
	])
])

AC_CHECK_PROG(GPERF_CHECK,gperf,yes)
if test x"$GPERF_CHECK" != x"yes" ; then
    AC_MSG_ERROR([Please install gperf before installing.])
fi

plugindir="\$(libdir)/camera-1.0"
AC_SUBST(plugindir)

AC_CHECK_HEADER_STDBOOL()

AC_TYPE_SIZE_T
AC_TYPE_UINT64_T

AC_CHECK_FUNCS([localtime_r])


AC_OUTPUT(Makefile 
	src/Makefile
	platforms/Makefile
	platforms/example/Makefile
	platforms/PI2Camera/Makefile

)


