GENERATES=labyrinth labyrinth0* labyrinth1* labyrinth2*
SOURCE_FILE=labyrinth.c

all: labyrinth labyrinth0 labyrinth1 labyrinth2

labyrinth%: labyrinth%.c
	cc -o $@ $<

labyrinth0.c: $(SOURCE_FILE) 0.patch
	patch -o $@ $< 0.patch

labyrinth1.c: labyrinth0.c 1.patch
	patch -o $@ $< 1.patch

labyrinth2.c: labyrinth1.c 2.patch
	patch -o $@ $< 2.patch

run: labyrinth labyrinth0 labyrinth1 labyrinth2
	./labyrinth
	./labyrinth0 7
	./labyrinth1 '%^' 8
	./labyrinth2 45 '01' 9

clean:
	rm $(GENERATES)
