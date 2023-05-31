SRC_DIR=src
INC_DIR=src

CC=gcc
CFLAGS=-I./${INC_DIR} -fsanitize=address -Wall -Wextra -Werror

SRCS=$(wildcard ${SRC_DIR}/*.c)

.PHONY: test clean

test: test_vector

test_vector:
	${CC} ${CFLAGS} test/test_vector.c ${SRCS}
