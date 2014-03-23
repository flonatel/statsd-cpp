bin_PROGRAMS += bin/statsd

bin_statsd_SOURCES = \
	src/main.cc

bin_statsd_CPPFLAGS = \
	-I$(top_srcdir)/${PEGTL_INCLUDE_DIR}

# Local Variables:
# mode: makefile
# End:
