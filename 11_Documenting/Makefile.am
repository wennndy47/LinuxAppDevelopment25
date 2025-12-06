SUBDIRS = src po

@DX_RULES@

all-local: doxygen-doc

doxygen-doc/man/man1/guesser.1: doxygen-doc
	help2man -N ./src/guesser > $@

man1_MANS = doxygen-doc/man/man1/guesser.1

clean_generates:
	rm -rf ${PO_GENERATES}
	rm -rf ${GENERATES}
    
http:	doxygen-doc
	python3 -m http.server --directory $</html