#include "test.h"
#include "map.h"
#include <stdio.h>

void test_int();

int main()
{
  TEST_PRINT_FILE();

  TestFunction tf[] = {
    test_int,
    NULL
  };

  for (int i = 0; tf[i] != NULL; i++) {
    tf[i]();
    printf("\n");
  }
}

void test_int()
{
  TEST_PRINT_FUNC();

  // map_t *map = new_map();
}