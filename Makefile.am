#
# This is the Master-Makefile
#
# Rules:
#  o NEVER EVER use recursive Make (cd xxx && make)
#    Use include instead
#  o Do not put rules / dependencies in this file:
#    Instead use the Makefile parts and include them
#    from here
#  o If unsure: ask!
#

ACLOCAL_AMFLAGS=-I m4

bin_PROGRAMS = 
lib_LTLIBRARIES =
noinst_PROGRAMS =
noinst_LTLIBRARIES =

BUILT_SOURCES =
CLEANFILES =
TESTS = 

# Generics
include contrib/MakefileGTest.inc
include build/MakefileBlackbox.inc
include src/Makefile.inc

# Librariy
include lib/statsdcpp/Makefile.inc

# Unittests
include lib/statsdcpp/unittests/Makefile.inc

# Performancetests
include lib/statsdcpp/performancetests/Makefile.inc
