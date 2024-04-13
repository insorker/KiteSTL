#ifndef TEST_H
#define TEST_H

#include <assert.h>
#include <stdio.h>

#define TEST_PRINT_FILE() printf("== TEST FILE: %s ==\n\n", __FILE__)
#define TEST_PRINT_FUNC() printf("--- TEST FUNC: %s ---\n", __func__)
#define TEST_ASSERT(expr) while ((expr) == 0) { perror("Err\n"); assert(0); }

typedef void (*test_t)();

void test_overall(test_t tf[]);

#endif
