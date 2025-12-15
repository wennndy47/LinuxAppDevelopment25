#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([The Hangman game], [1.0], [me@me.ru], [hangman])
AM_INIT_AUTOMAKE([foreign])
DX_INIT_DOXYGEN(hangman)
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_REQUIRE_VERSION(0.21)

AC_CONFIG_SRCDIR([src/hangman.c])
AC_CONFIG_HEADERS([config.h])
# AC_CONFIG_MACRO_DIRS([m4])
# AC_SUBST(GETTEXT_PACKAGE, "$PACKAGE")
# ALL_LINGUAS="ru"

AC_PROG_CC
# IT_PROG_INTLTOOL
LT_INIT

# AC_ARG_WITH([help2man-locale],
#     [AS_HELP_STRING([--with-help2man-locale=LOCALE],
#         [locale to use for help2man (default: C)])],
#     [HELP2MAN_LOCALE="$withval"],
#     [HELP2MAN_LOCALE="C"])
# AC_SUBST([HELP2MAN_LOCALE])

AC_CHECK_HEADERS([libintl.h locale.h stdlib.h])
AC_TYPE_SIZE_T
AC_CHECK_FUNCS([setlocale])

PKG_CHECK_MODULES([CHECK], [check])

AC_CONFIG_FILES([
    Makefile
    po/Makefile
    src/Makefile
    tests/Makefile
    Doxyfile
])

AC_OUTPUT