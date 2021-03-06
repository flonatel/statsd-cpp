#!/bin/bash
#
# Removes all the autotools artifacts.
#

rm -fr Makefile.in aclocal.m4 ar-lib \
    autom4te.cache compile config.guess \
    config.h.in config.sub configure \
    install-sh ltmain.sh m4/libtool.m4 \
    m4/lt~obsolete.m4 m4/ltoptions.m4 \
    m4/ltsugar.m4 m4/ltversion.m4 missing \
    depcomp

