TESTS = worddict
check_PROGRAMS = worddict

.ts.c:
	checkmk $< > $@

AM_CFLAGS = -I$(top_builddir)/src @CHECK_CFLAGS@
LDADD = $(top_builddir)/src/libworddict.la @CHECK_LIBS@
