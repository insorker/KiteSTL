#ifndef TEST_H
#define TEST_H

#define TEST_PRINT_FILE() printf("== TEST FILE: %s ==\n\n", __FILE__)
#define TEST_PRINT_FUNC() printf("--- TEST FUNC: %s ---\n", __func__)

typedef void (*TestFunction)();

#endif
