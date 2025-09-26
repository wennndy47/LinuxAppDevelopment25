GENERATES = prog README
TRASH = *.o *~ o.*
CFLAGS = -Wall

all:	README prog

prog:	const.o fun.o prog.o
	cc $^ -o $@

README: prog
	./$< 2> $@

fun.o: outlib.h

clean:
	rm -f $(TRASH)

distclean:	clean
	rm -rf $(GENERATES)
