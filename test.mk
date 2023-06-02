.PHONY: test

test: test_vector

test_vector:
	${CC} ${CFLAGS} test/test_vector.c ${SRCS}
