dnl -*-m4-*-
dnl Process this file with autoconf to produce a configure script.
dnl --------------------------------------------------------------
dnl configure.in for Worldsens network simulator
dnl --------------------------------------------------------------
AC_INIT([wsnet-2.0],[0.7cvs])
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

dnl --------------------------------------------------------------
dnl Install directory
dnl --------------------------------------------------------------
AC_PREFIX_DEFAULT(/usr/local/wsnet-2.0)
CFLAGS="-DDATADIR='\"${datadir}\"' $CFLAGS"
CFLAGS="-DLIBDIR='\"${libdir}\"' $CFLAGS"
CFLAGS="-fno-strict-aliasing -Wall -Wextra -Wno-unused-parameter -Werror $CFLAGS"

dnl --------------------------------------------------------------
dnl Checks for pkg-config path.
dnl --------------------------------------------------------------
AC_ARG_VAR(PKG_CONFIG_DIR, [pkg-config directory])
AC_ARG_WITH([pkg-config-dir],
	    AS_HELP_STRING([--with-pkg-config-dir=DIR],[pkg-config directory]),
	    [PKG_CONFIG_DIR=$withval],
	    [PKG_CONFIG_DIR=""])

dnl --------------------------------------------------------------
dnl Checks for pkg-config.
dnl --------------------------------------------------------------
AC_PATH_PROG([PKG_CONFIG], [pkg-config], , [$PATH:$PKG_CONFIG_DIR])
AC_ARG_VAR(GLIB_LIBS, [glib libraries])
AC_ARG_VAR(GLIB_FLAGS, [glib flags])
if test "${GLIB_LIBS}" = ""; then
	[GLIB_LIBS]=`$PKG_CONFIG --libs gmodule-2.0`
	echo $GLIB_LIBS
else
	echo $GLIB_LIBS
fi

if test "${GLIB_FLAGS}" = ""; then
	[GLIB_FLAGS]=`$PKG_CONFIG --cflags gmodule-2.0`
	echo $GLIB_FLAGS
else
	echo $GLIB_FLAGS
fi

AC_ARG_VAR(XML_LIBS, [xml libraries])
AC_ARG_VAR(XML_FLAGS, [xml flags])
if test "${XML_LIBS}" = ""; then
	[XML_LIBS]=`$PKG_CONFIG --libs libxml-2.0`
	echo $XML_LIBS
else
	echo $XML_LIBS
fi

if test "${XML_FLAGS}" = ""; then
	[XML_FLAGS]=`$PKG_CONFIG --cflags libxml-2.0`
	echo $XML_FLAGS
else
	echo $XML_FLAGS
fi

AC_ARG_VAR(GSL_LIBS, [gsl libraries])
AC_ARG_VAR(GSL_FLAGS, [gsl flags])
if test "${GSL_LIBS}" = ""; then
	[GSL_LIBS]=`$PKG_CONFIG --libs gsl`
	echo $GSL_LIBS
else
	echo $GSL_LIBS
fi

if test "${GSL_FLAGS}" = ""; then
	[GSL_FLAGS]=`$PKG_CONFIG --cflags gsl`
	echo $GSL_FLAGS
else
	echo $GSL_FLAGS
fi


dnl --------------------------------------------------------------
dnl Simulator internals options.
dnl --------------------------------------------------------------
AC_ARG_WITH([mem-fs], AS_HELP_STRING([--with-mem-fs=policy],[Fixed size memory allocation policy (default policy is prealloc)]), [MEM_FS=$withval], [MEM_FS="prealloc"])
AC_SUBST(MEM_FS)

AC_ARG_WITH([das], AS_HELP_STRING([--with-das=structure],[Unsorted data structure (default structure is a list)]), [DAS=$withval], [DAS="list"])
AC_SUBST(DAS)

AC_ARG_WITH([sodas], AS_HELP_STRING([--with-sodas=structure],[Sorted data structure (default structure is a heap)]), [SODAS=$withval], [SODAS="heap"])
AC_SUBST(SODAS)

AC_ARG_WITH([spadas], AS_HELP_STRING([--with-spadas=structure],[Spatial data structure (default structure is a list)]), [SPADAS=$withval], [SPADAS="grid"])
AC_SUBST(SPADAS)

AC_ARG_WITH([hadas], AS_HELP_STRING([--with-hadas=structure],[Hashed data structure (default structure is a  hash table of lists)]), [HADAS=$withval], [HADAS="hash"])
AC_SUBST(HADAS)

AC_ARG_WITH([scheduler], AS_HELP_STRING([--with-scheduler=support],[Scheduler support (default support is local nodes only)]), [SCHEDULER=$withval], [SCHEDULER="distant"])
AC_SUBST(SCHEDULER)

AC_ARG_ENABLE([optimize-nodes], AS_HELP_STRING([--enable-optimize-nodes],[Optimize node management with spatial data structures]), CFLAGS="-D N_DAS_O $CFLAGS",)

AC_ARG_ENABLE([periodic-mobility], AS_HELP_STRING([--enable-periodic-mobility],[Set periodic mobility instead of event driven]), CFLAGS="-D MOB_PERIOD $CFLAGS",)

AC_ARG_ENABLE([debug], AS_HELP_STRING([--enable-debug],[Set debug compilation options]), CFLAGS="-g -pg $CFLAGS",CFLAGS="-O2 $CFLAGS")


dnl --------------------------------------------------------------
dnl Checks for programs.
dnl --------------------------------------------------------------
AC_PROG_INSTALL
AC_LIBTOOL_DLOPEN
AC_PROG_LIBTOOL
AC_PROG_CC
AM_PROG_CC_C_O
AM_PROG_GCJ

dnl --------------------------------------------------------------
dnl Output
dnl --------------------------------------------------------------
AC_CONFIG_FILES(
include/Makefile

libraries/mem_fs/malloc/Makefile
libraries/mem_fs/prealloc/Makefile
libraries/mem_fs/Makefile
libraries/das/list/Makefile
libraries/das/Makefile
libraries/sodas/list/Makefile
libraries/sodas/heap/Makefile
libraries/sodas/Makefile
libraries/timer/Makefile
libraries/hadas/hash/Makefile
libraries/hadas/Makefile
libraries/spadas/dbtree/Makefile
libraries/spadas/flat/Makefile
libraries/spadas/grid/Makefile
libraries/spadas/Makefile
libraries/worldsens/Makefile
libraries/Makefile

models/propagation/Makefile
models/interferences/Makefile
models/modulation/Makefile
models/environment/Makefile
models/monitor/Makefile
models/application/data_d/lbdd/Makefile
models/application/data_d/xy/Makefile
models/application/data_d/ght/Makefile
models/application/data_d/gossip/Makefile
models/application/data_d/Makefile
models/application/Makefile
models/routing/Makefile
models/mac/Makefile
models/radio/Makefile
models/antenna/Makefile
models/mobility/Makefile
models/energy/Makefile
models/noise/Makefile
models/Makefile

src/scheduler/local/Makefile
src/scheduler/distant/Makefile
src/scheduler/Makefile
src/Makefile

utils/topogen/Makefile
utils/replay/Makefile
utils/matlab_functions/maps/Makefile
utils/matlab_functions/Makefile
utils/gnuplot_functions/activenodes/Makefile
utils/gnuplot_functions/Makefile
utils/scripts/Makefile
utils/Makefile

demo/Makefile

doc/module_dev/Makefile
doc/Makefile

Makefile
)
AC_OUTPUT

dnl --------------------------------------------------------------
dnl Summary
dnl --------------------------------------------------------------
echo ""
echo "Configuration summary:"
echo ""
echo "        fs memory allocation     : $MEM_FS"
echo "        sorted data structure    : $SODAS"
echo "        unsorted data structure  : $DAS"
echo "        spatial data structure   : $SPADAS"
echo "        hashed data structure    : $HADAS"
echo "        scheduler                : $SCHEDULER"
echo "        optimize nodes using das : ${enable_optimize_nodes}"
echo "        periodic mobility        : ${enable_periodic_mobility}"
echo "        debug                    : ${enable_debug}"
echo ""
