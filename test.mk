.PHONY: test

test: vector_test map_test

vector_test:
	${CC} ${CFLAGS} test/vector_test.c ${SRCS} -o vector_test.out

map_test:
	${CC} ${CFLAGS} test/map_test.c ${SRCS} -o map_test.out