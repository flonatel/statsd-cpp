#!/bin/bash
#
# This must be called from within the top source dir.
#

libtoolize --copy

aclocal
autoheader
automake --add-missing --copy
automake
autoconf
