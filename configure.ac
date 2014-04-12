AC_INIT([statsd-cpp], [0.1], [info@flonatel.org])

AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
AC_CONFIG_MACRO_DIR([m4])
AM_SILENT_RULES([yes])
AM_PROG_AR
LT_INIT
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_CXX

CXXFLAGS="-std=c++11 -Wall -Wextra -Werror"

# debug compilation support
AC_MSG_CHECKING([whether to build with debug information])
AC_ARG_ENABLE([debug],
    [AS_HELP_STRING([--enable-debug],
        [enable debug data generation (def=no)])],
    [debugit="$enableval"],
    [debugit=no])
AC_MSG_RESULT([$debugit])

if test x"$debugit" = x"yes"; then
    AC_DEFINE([WSS_DEBUG],[],[Debug Mode])
    CXXFLAGS="$CXXFLAGS -ggdb"
else
    AC_DEFINE([WSS_NDEBUG],[],[No-debug Mode])
    CXXFLAGS="$CXXFLAGS -O3"
fi

# profiling
AC_MSG_CHECKING([whether to build with profiling support])
AC_ARG_ENABLE([profiling],
    [AS_HELP_STRING([--enable-profiling],
        [enable profiling (def=no)])],
    [profiling="$enableval"],
    [profiling=no])
AC_MSG_RESULT([$profiling])

if test x"$profiling" = x"yes"; then
    AC_DEFINE([WSS_PROFILING],[],[Profiling Mode])
    CXXFLAGS="$CXXFLAGS -pg"
    LDFLAGS="${LDFLAGS} -pg"
fi

# coverage compilation support
AC_MSG_CHECKING([whether to build with coverage])
AC_ARG_ENABLE([coverage],
    [AS_HELP_STRING([--enable-coverage],
        [enable coverage data generation (def=no)])],
    [coverit="$enableval"],
    [coverit=no])
AC_MSG_RESULT([$coverit])

# Support for running test cases using valgrind:
use_valgrind=false
AC_ARG_ENABLE(valgrind,
    [  --enable-valgrind       Use valgrind when running unit tests. ],
    [ use_valgrind=true ])
                 
if test "$use_valgrind" = "true" ; then
   AC_CHECK_PROG(HAVE_VALGRIND, valgrind, yes, no)
                 
   if test "$HAVE_VALGRIND" = "no" ; then
       AC_MSG_ERROR([Valgrind not found in PATH. ])
   fi
fi
AC_SUBST(HAVE_VALGRIND)
AM_CONDITIONAL(USE_VALGRIND, $use_valgrind)

if test x"$coverit" = x"yes"; then
    AC_DEFINE([WSS_COVERAGE],[],[Coverage Mode])
    CXXFLAGS="$CXXFLAGS --coverage"
    LIBS="${LIBS} -lgcov"
else
    AC_DEFINE([WSS_NCOVERAGE],[],[No-coverage Mode])
fi

PEGTL_BASE=contrib/pegtl
PEGTL_INCLUDE_DIR=${PEGTL_BASE}/include
AC_SUBST(PEGTL_BASE)
AC_SUBST(PEGTL_INCLUDE_DIR)

PTL_BASE=contrib/ptl
PTL_INCLUDE=${PTL_BASE}/lib
AC_SUBST(PTL_BASE)
AC_SUBST(PTL_INCLUDE)

GOOGLE_TEST_BASE=contrib/gtest
GOOGLE_TEST_INCLUDE=${GOOGLE_TEST_BASE}/include
AC_SUBST(GOOGLE_TEST_BASE)
AC_SUBST(GOOGLE_TEST_INCLUDE)

AX_PTHREAD()
LIBS="$PTHREAD_LIBS $LIBS"
CFLAGS="$CFLAGS $PTHREAD_CFLAGS"
CXXFLAGS="$CXXFLAGS $PTHREAD_CFLAGS"

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
 Makefile
])
AC_OUTPUT
