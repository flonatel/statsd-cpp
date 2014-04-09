lib_LTLIBRARIES += lib/libstatsdcpp.la

lib_libstatsdcpp_la_SOURCES = \
	lib/statsdcpp/counter.cc 

lib_libstatsdcpp_la_CPPFLAGS = \
        -I$(top_srcdir)/lib

# Local Variables:
# mode: makefile
# End:

