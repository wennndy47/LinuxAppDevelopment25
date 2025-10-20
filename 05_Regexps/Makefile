CFLAGS=-Wall -O0 -g 
GENERATES=esub
TRASH=result* expected*

all: esub

test: esub
	@echo "Start testing"
	echo "eabc" | sed -E "s/ab/BA/" > expected_1
	echo "DabcE" | sed -E "s/a(b)c/\1_\1/" > expected_2
	date | sed -E "s/([[:digit:]]+)(.*)([[:digit:]]+)/<1=\1><2=\2><3=\3>/" > expected_3
	./esub "ab" "BA" "eabc" > result_1
	./esub "a(b)c" "\1_\1" "DabcE" > result_2
	./esub "([[:digit:]]+)(.*)([[:digit:]]+)" "<1=\1><2=\2><3=\3>"  "`date`" > result_3
	cmp expected_1 result_1
	cmp expected_2 result_2
	cmp expected_3 result_3
	@echo "Tests done"
	
clean:
	rm -f $(GENERATES) $(TRASH)
