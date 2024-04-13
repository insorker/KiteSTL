#include "test.h"

void test_overall(test_t tf[])
{
  for (int i = 0; tf[i]; i++) {
    tf[i]();
    printf("OK\n");
  }
}