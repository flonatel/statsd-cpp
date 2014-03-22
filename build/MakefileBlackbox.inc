.PHONY: bbtests

bbtests:
	nosetests -v --with-xunit --xunit-file=blackboxtests.xml \
	  --processes=7 --process-timeout=180 \
	  --testmatch="BBT_.*|bbt_.*" -w . \
	   -w ${top_srcdir}/blackbox

# Local Variables:
# mode: makefile
# End:
