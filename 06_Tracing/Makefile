CFLAGS=-Wall
GENERATES=move *.in *.out protect.so
TRASH= *.o

all: move protect.so

protect.so: protect.c
	cc -shared -fPIC $^ -o $@

test_error: move
	@echo "Test strace"
	@echo "test input" > 1.in 
	
	@strace -e fault=openat:error=ENOENT -P "1.in" ./move "1.in" "1.out"; \
	if [ $$? -eq 3 ] && [ -f 1.in ] && [ ! -f 1.out ]; then \
		echo "Test 1 passed"; \
	else \
		echo "Test 1 failed"; \
		exit 1; \
	fi
	
	@echo "test input 2" > 2.in 
	@strace -e fault=openat:error=EACCES -P "2.in" ./move "2.in" "2.out"; \
	if [ $$? -eq 3 ] && [ -f 2.in ] && [ ! -f 2.out ]; then \
		echo "Test 2 passed"; \
	else \
		echo "Test 2 failed"; \
		exit 1; \
	fi
	
	@echo "test input 3" > 3.in 
	@strace -e fault=read:error=EIO -P "3.in" ./move "3.in" "3.out"; \
	if [ $$? -eq 3 ] && [ -f 3.in ] && [ ! -f 3.out ]; then \
		echo "Test 3 passed"; \
	else \
		echo "Test 3 failed"; \
		exit 3; \
	fi
	
	@echo "test input 4" > 4.in 
	@strace -e fault=openat:error=ENOSPC -P "4.out" ./move "4.in" "4.out"; \
	if [ $$? -eq 4 ] && [ -f 4.in ] && [ ! -f 4.out ]; then \
		echo "Test 4 passed"; \
	else \
		echo "Test 4 failed"; \
		exit 4; \
	fi
	
	@echo "test input 5" > 5.in 
	@strace -e write -e fault=write:error=EIO -P "5.out" ./move "5.in" "5.out"; \
	if [ $$? -eq 9 ] && [ -f 5.in ] && [ -f 5.out ]; then \
		echo "Test 5 passed"; \
	else \
		echo "Test 5 failed"; \
		exit 5; \
	fi
	
	@echo "test input 6" > 6.in 
	@strace -e fault=close:error=EIO -P "6.in" ./move "6.in" "6.out"; \
	if [ $$? -eq 7 ] && [ -f 6.in ] && [ ! -f 6.out ]; then \
		echo "Test 6 passed"; \
	else \
		echo "Test 6 failed"; \
		exit 6; \
	fi
	
	@echo "test input 7" > 7.in 
	@strace -e fault=unlink:error=EACCES -P "7.in" ./move "7.in" "7.out"; \
	if [ $$? -eq 10 ] && [ -f 7.in ] && [ -f 7.out ]; then \
		echo "Test 7 passed"; \
	else \
		echo "Test 7 failed"; \
		exit 7; \
	fi
	
	@echo "Test strace passed"

test_preload: move protect.so
	@echo "Test LD_PRELOAD"
	@echo "Protected. Should not be erased" > PROTECTed.in 
	LD_PRELOAD=`pwd`/protect.so ./move "PROTECTed.in" "preload.out" > /dev/null 2>&1 || \
	if [ -f PROTECTed.in ] && [ ! -f preload.out ]; then \
		echo "Test passed"; \
	else \
		echo "Test failed"; \
		exit 1; \
	fi
	@echo "Test LD_PRELOAD passed"

test: test_error test_preload
	
clean:
	rm -f $(GENERATES) $(TRASH)
