include Makefile.lst

CFLAGS= -W -Wall -O2
LIBS=-s `allegro-config --libs`
OBJ=$(SRC:.c=.o)

all: main

# :D
${OBJ}: ${HDR}

main: ${OBJ}
	gcc $^ -o allebob ${CFLAGS} ${LIBS}

%.o: %.c
	gcc ${CFLAGS} -c $< -o $@

clean:
	rm -rf allebob
	rm -rf src/*.o
