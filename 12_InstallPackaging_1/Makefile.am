ACLOCAL_AMFLAGS=-I m4
SUBDIRS = src po tests

@DX_RULES@

all-local: doxygen-doc

doxygen-doc/man/man1/hangman.1: doxygen-doc
	help2man -N src/hangman > $@

man1_MANS=doxygen-doc/man/man1/hangman.1

install-data-local:
	mkdir -p $(DESTDIR)/$(docdir)
	cp -a doxygen-doc/html $(DESTDIR)/$(docdir)

http:	doxygen-doc
	python3 -m http.server --directory $</html
