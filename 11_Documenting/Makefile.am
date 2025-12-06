SUBDIRS = src po

@DX_RULES@

all-local: doxygen-doc

clean_generates:
	rm -rf ${PO_GENERATES}
	rm -rf ${GENERATES}
    
http:	doxygen-doc
	python3 -m http.server --directory $</html