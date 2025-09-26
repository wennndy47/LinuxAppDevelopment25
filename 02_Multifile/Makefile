GENERATES = prog README prog-a prog-so liboutput.so liboutput_static.a
TRASH = *.o *~ o.* result_*
CFLAGS = -fPIC

all:	README prog prog-a prog-so

liboutput_static.a: liboutput_static.a(fun.o const.o)

liboutput.so: fun.o const.o
	cc -shared $^ -o $@

prog:	const.o fun.o prog.o

prog-a: prog.o liboutput_static.a
	cc -L. $< -loutput_static -o $@

prog-so: prog.o liboutput.so
	cc -L. $< -loutput -o $@

README: prog
	./$< 2> $@

fun.o prog.o const.o: outlib.h

test: prog prog-a prog-so
	./prog > result_prog_0 2>&1
	./prog-a > result_prog-a_0 2>&1
	LD_LIBRARY_PATH=$(CURDIR) ./prog-so > result_prog-so_0 2>&1
	cmp result_prog_0 result_prog-a_0
	cmp result_prog_0 result_prog-so_0
	cmp result_prog-a_0 result_prog-so_0
	
	./prog A > result_prog_1 2>&1
	./prog-a A > result_prog-a_1 2>&1
	LD_LIBRARY_PATH=$(CURDIR) ./prog-so A > result_prog-so_1 2>&1
	cmp result_prog_1 result_prog-a_1
	cmp result_prog_1 result_prog-so_1
	cmp result_prog-a_1 result_prog-so_1
	
	./prog A BB CCC > result_prog_2 2>&1
	./prog-a A BB CCC > result_prog-a_2 2>&1
	LD_LIBRARY_PATH=$(CURDIR) ./prog-so A BB CCC > result_prog-so_2 2>&1
	cmp result_prog_2 result_prog-a_2
	cmp result_prog_2 result_prog-so_2
	cmp result_prog-a_2 result_prog-so_2

clean:
	rm -f $(TRASH)

distclean:	clean
	rm -rf $(GENERATES)
