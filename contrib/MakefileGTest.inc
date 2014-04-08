noinst_LTLIBRARIES += contrib/gtest/lib/libgtest.la

contrib_gtest_lib_libgtest_la_SOURCES = \
        contrib/gtest/src/gtest-all.cc

contrib_gtest_lib_libgtest_la_CPPFLAGS = \
        -I$(top_srcdir)/${GOOGLE_TEST_BASE} \
        -I$(top_srcdir)/${GOOGLE_TEST_INCLUDE}

# Local Variables:
# mode: makefile
# End:
