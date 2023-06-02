SRC_DIR=src
INC_DIR=inc

CC=gcc
CFLAGS=-I./${INC_DIR} -fsanitize=address -Wall -Werror
LDFLAGS=-shared

TARGET_DIR=lib
TARGET=${TARGET_DIR}/libKiteSTL.so

SRCS=$(wildcard ${SRC_DIR}/*.c)
OBJS=$(patsubst $(SRC_DIR)/%.c, $(TARGET_DIR)/%.o, $(SRCS))

.PHONY: all clean

all: ${TARGET}

${TARGET}: ${OBJS}
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^

${TARGET_DIR}/%.o: ${SRC_DIR}/%.c
	@mkdir -p ${TARGET_DIR}
	${CC} ${CFLAGS} -fPIC -c -o $@ $<

clean:
	rm -rf *.out ${TARGET_DIR}

include test.mk
