CFLAGS = -O0 -g
GENERATES = range actual-1.txt actual-2.txt

all: range

test-%: range
	gdb --batch --quiet range -x scenario-$*.gdb | grep @@@ > actual-$*.txt
	cmp actual-$*.txt expected-$*.txt

test: test-1 test-2

clean:
	rm -f $(GENERATES)