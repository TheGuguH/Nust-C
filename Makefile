exec = nscomp.out
sources = $(wildcard src/*.c) $(wildcard src/front-end/*.c) $(wildcard src/back-end/*.c)
objects = $(sources:.c=.o)
flags = -g

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
	-rm src/front-end/*.o
	-rm src/back-end/*.o
