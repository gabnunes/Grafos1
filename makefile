CC=gcc
CFLAGS=-Wall
LDFLAGS = -lm

a: a.o
	${CC} ${CFLAGS} a.o ${LDFLAGS} -o a

clean:
	\rm a.o a
