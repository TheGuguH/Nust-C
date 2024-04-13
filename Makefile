exec = nct.out
sources = $(wildcard src/*.c) $(wildcard src/compiler/front-end/*.c) $(wildcard src/compiler/back-end/*.c)
objects = $(sources:.c=.o)
flags = -g -Wall -Wextra -std=c99 -I./headers

$(exec): $(objects)
	gcc $(objects) $(flags) -o $(exec)

%.o: %.c include/%.h
	gcc -c $(flags) $< -o $@

install:
	make
	cp ./nscomp.out /usr/local/bin/nscomp

clean:
	-rm *.out
	-rm *.o
	-rm src/*.o
	-rm src/compiler/front-end/*.o
	-rm src/compiler/back-end/*.o
