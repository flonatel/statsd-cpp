noinst_PROGRAMS += tests/unit/CorrelatorPerformance

tests_unit_CorrelatorPerformance_SOURCES = \
	lib/statsdcpp/performancetests/CorrelatorPerformance.cc

tests_unit_CorrelatorPerformance_CPPFLAGS = \
	-I$(top_srcdir)/${PTL_INCLUDE} \
	-I$(top_srcdir)/${GOOGLE_TEST_INCLUDE} \
	-I$(top_srcdir)/lib

tests_unit_CorrelatorPerformance_LDADD = \
	lib/libstatsdcpp.la \
	contrib/gtest/lib/libgtest.la

# Local Variables:
# mode: makefile
# End:
