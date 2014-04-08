noinst_PROGRAMS += tests/unit/CounterSimpleTest

TESTS += tests/unit/CounterSimpleTest

tests_unit_CounterSimpleTest_SOURCES = \
        lib/cpp/unittests/CounterSimpleTest.cc

tests_unit_CounterSimpleTest_CPPFLAGS = \
        -Ilib

tests_unit_CounterSimpleTest_LDADD = \
        lib/libstatsd++.la
