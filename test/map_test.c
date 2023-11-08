#include "test.h"
#include "map.h"
#include <stdio.h>

void test_int_int();
void test_char_int();
void test_char_char();

int main()
{
  TEST_PRINT_FILE();

  TestFunction tf[] = {
    test_int_int,
    test_char_int,
    test_char_char,
    NULL
  };

  for (int i = 0; tf[i] != NULL; i++) {
    tf[i]();
    printf("\n");
  }
}

void test_int_int()
{
  TEST_PRINT_FUNC();

  map_t *map = new_map(treap_utils.tu_int, treap_utils.tu_int);

  map->insert(map, &(int){1}, &(int){1});
  map->insert(map, &(int){2}, &(int){2});
  printf("%d\n", *(int *)map->find(map, &(int){1}));
  printf("%d\n", *(int *)map->find(map, &(int){2}));
  if (map->find(map, &(int){2}) == NULL) { printf("can not find 2\n"); }
  if (map->find(map, &(int){3}) == NULL) { printf("can not find 3\n"); }

  map->insert(map, &(int){2}, &(int){22});
  printf("%d\n", *(int *)map->find(map, &(int){2}));

  map->erase(map, &(int){3});
  map->erase(map, &(int){1});
  if (map->find(map, &(int){1}) == NULL) { printf("can not find 1\n"); }

  free_map(map);
}

void test_char_int()
{
  TEST_PRINT_FUNC();

  map_t *map = new_map(treap_utils.tu_char, treap_utils.tu_int);

  map->insert(map, "John", &(int){100});
  map->insert(map, "Mary", &(int){98});
  map->insert(map, "David", &(int){70});
  printf("%d\n", *(int *)map->find(map, "John"));
  printf("%d\n", *(int *)map->find(map, "Mary"));
  if (map->find(map, "David") == NULL) { printf("can not find David\n"); }
  if (map->find(map, "Tom") == NULL) { printf("can not find Tom\n"); }

  map->erase(map, "Tom");
  map->erase(map, "John");
  if (map->find(map, "John") == NULL) { printf("can not find John\n"); }
  printf("%d\n", *(int *)map->find(map, "David"));

  free_map(map);
}

void test_char_char()
{
  TEST_PRINT_FUNC();

  map_t *map = new_map(treap_utils.tu_char, treap_utils.tu_char);

  for (char c = 'a'; c <= 'z'; c++) {
    char cc[2];
    cc[0] = c, cc[1] = '\0';

    map->insert(map, cc, cc);
  }

  for (char c = 'a'; c <= 'z'; c++) {
    char cc[2];
    cc[0] = c, cc[1] = '\0';

    if (map->find(map, cc) != NULL) {
      printf("%s", (char *)map->find(map, cc));
    }
  }

  free_map(map);
}