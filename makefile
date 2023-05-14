SRC_DIRS:=src
INC_DIRS:=-I./src

CC:=gcc
CFLAGS:=${INC_DIRS} -g -fsanitize=address -Wall -Werror
SOURCES:=$(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.c))

all: test

test: test_vector

test_vector:
	${CC} ${CFLAGS} test/test_vector.c ${SOURCES}

clean:
	rm -f *.out
