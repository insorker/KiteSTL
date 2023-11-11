#ifndef TEST_H
#define TEST_H

#include <assert.h>
#include <stdio.h>

#define TEST_PRINT_FILE() printf("== TEST FILE: %s ==\n\n", __FILE__)
#define TEST_PRINT_FUNC() printf("--- TEST FUNC: %s --- : ", __func__)
#define TEST_ASSERT(expr, log) while ((expr) == 0) { printf("Wrong\n"); perror(log); assert(0); }

typedef void (*TestFunction)();

#endif
