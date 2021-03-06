noinst_PROGRAMS += tests/unit/CounterSimpleTest

TESTS += tests/unit/CounterSimpleTest

tests_unit_CounterSimpleTest_SOURCES = \
        lib/statsdcpp/unittests/CounterSimpleTest.cc

tests_unit_CounterSimpleTest_CPPFLAGS = \
        -I$(top_srcdir)/${GOOGLE_TEST_INCLUDE} \
        -I$(top_srcdir)/lib

tests_unit_CounterSimpleTest_LDADD = \
        lib/libstatsdcpp.la \
	contrib/gtest/lib/libgtest.la


noinst_PROGRAMS += tests/unit/MeasureSimpleTest

TESTS += tests/unit/MeasureSimpleTest

tests_unit_MeasureSimpleTest_SOURCES = \
        lib/statsdcpp/unittests/MeasureSimpleTest.cc

tests_unit_MeasureSimpleTest_CPPFLAGS = \
        -I$(top_srcdir)/${GOOGLE_TEST_INCLUDE} \
        -I$(top_srcdir)/lib

tests_unit_MeasureSimpleTest_LDADD = \
        lib/libstatsdcpp.la \
	contrib/gtest/lib/libgtest.la


noinst_PROGRAMS += tests/unit/QuantitiySimpleTest

TESTS += tests/unit/QuantitiySimpleTest

tests_unit_QuantitiySimpleTest_SOURCES = \
        lib/statsdcpp/unittests/QuantitiySimpleTest.cc

tests_unit_QuantitiySimpleTest_CPPFLAGS = \
        -I$(top_srcdir)/${GOOGLE_TEST_INCLUDE} \
        -I$(top_srcdir)/lib

tests_unit_QuantitiySimpleTest_LDADD = \
        lib/libstatsdcpp.la \
	contrib/gtest/lib/libgtest.la


noinst_PROGRAMS += tests/unit/CounterCarbonTest

TESTS += tests/unit/CounterCarbonTest

tests_unit_CounterCarbonTest_SOURCES = \
        lib/statsdcpp/unittests/CounterCarbonTest.cc

tests_unit_CounterCarbonTest_CPPFLAGS = \
        -I$(top_srcdir)/${GOOGLE_TEST_INCLUDE} \
        -I$(top_srcdir)/lib

tests_unit_CounterCarbonTest_LDADD = \
        lib/libstatsdcpp.la \
	contrib/gtest/lib/libgtest.la


noinst_PROGRAMS += tests/unit/CorrelatorTest

TESTS += tests/unit/CorrelatorTest

tests_unit_CorrelatorTest_SOURCES = \
        lib/statsdcpp/unittests/CorrelatorTest.cc

tests_unit_CorrelatorTest_CPPFLAGS = \
        -I$(top_srcdir)/${GOOGLE_TEST_INCLUDE} \
        -I$(top_srcdir)/lib

tests_unit_CorrelatorTest_LDADD = \
        lib/libstatsdcpp.la \
	contrib/gtest/lib/libgtest.la

# Local Variables:
# mode: makefile
# End:
