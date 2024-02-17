#include "test.h"
#include "map.h"
#include "vector.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>

const char *ERR_MAP_FIND = "Error -> map->find";
const char *ERR_MAP_INSERT = "Error -> map->insert";
const char *ERR_MAP_ERASE = "Error -> map->erase";
const char *ERR_MAP_SIZE = "Error -> map->size";

void test_int_int();
void test_str_int();
void test_str_str();
int pair_int_cmp(void *lhs, void *rhs);
void *pair_int_clone(void *val);
void pair_int_free(void *val);
void test_str_struct();

int main()
{
  TEST_PRINT_FILE();

  TestFunction tf[] = {
    test_int_int,
    test_str_int,
    test_str_str,
    test_str_struct,
    NULL
  };

  for (int i = 0; tf[i] != NULL; i++) {
    tf[i](); printf("OK \n");
  }
}

void test_int_int()
{
  TEST_PRINT_FUNC();

  map_t *map = new_map(cemu_int(), cemu_int());

  map->insert(map, &(int){1}, &(int){1});
  map->insert(map, &(int){2}, &(int){2});

  TEST_ASSERT(*(int *)map->find(map, &(int){1}) == 1, ERR_MAP_FIND);
  TEST_ASSERT(*(int *)map->find(map, &(int){2}) == 2, ERR_MAP_FIND);
  TEST_ASSERT(map->find(map, &(int){3}) == NULL, ERR_MAP_FIND);

  map->insert(map, &(int){2}, &(int){22});

  TEST_ASSERT(*(int *)map->find(map, &(int){2}) == 22, ERR_MAP_INSERT);
  TEST_ASSERT(map->size(map) == 2, ERR_MAP_SIZE);

  map->erase(map, &(int){3});
  map->erase(map, &(int){1});

  TEST_ASSERT(map->find(map, &(int){1}) == NULL, ERR_MAP_ERASE);

  map->clear(map);
  TEST_ASSERT(map->size(map) == 0, ERR_MAP_SIZE);

  for (int i = 0; i < 100; i++) {
    map->insert(map, &(int){i}, &(int){i});
    TEST_ASSERT(map->size(map) == i + 1, ERR_MAP_SIZE);
  }

  free_map(map);
}

void test_str_int()
{
  // TEST_PRINT_FUNC();

  // map_t *map = new_map(treap_cemu_pchar, treap_cemu_int);

  // map->insert(map, &(char *){"John"}, &(int){100});
  // map->insert(map, &(char *){"Mary"}, &(int){98});
  // map->insert(map, &(char *){"David"}, &(int){70});
  // TEST_ASSERT(map->size(map) == 3, ERR_MAP_SIZE);

  // TEST_ASSERT(*(int *)map->find(map, &(char *){"John"}) == 100, ERR_MAP_FIND);
  // TEST_ASSERT(*(int *)map->find(map, &(char *){"Mary"}) == 98, ERR_MAP_FIND);

  // TEST_ASSERT(*(int *)map->find(map, &(char *){"David"}) == 70, ERR_MAP_FIND);
  // TEST_ASSERT(map->find(map, &(char *){"Tom"}) == NULL, ERR_MAP_FIND);

  // map->erase(map, &(char *){"Tom"});
  // map->erase(map, &(char *){"John"});
  // TEST_ASSERT(map->size(map) == 2, ERR_MAP_SIZE);

  // TEST_ASSERT(map->find(map, &(char *){"John"}) == NULL, ERR_MAP_ERASE);
  // TEST_ASSERT(*(int *)map->find(map, &(char *){"David"}) == 70, ERR_MAP_ERASE);

  // free_map(map);
}

void test_str_str()
{
  // TEST_PRINT_FUNC();

  // map_t *map = new_map(treap_cemu_pchar, treap_cemu_pchar);

  // for (char c = 'a'; c <= 'z'; c++) {
  //   char cc[] = { c, '\0' };

  //   map->insert(map, &(char *){cc}, &(char *){cc});
  // }

  // for (char c = 'a'; c <= 'z'; c++) {
  //   char cc[2] = { c, '\0' };

  //   TEST_ASSERT(
  //     strcmp(*(char **)map->find(map, &(char *){cc}), cc) == 0,
  //     ERR_MAP_INSERT
  //   );
  // }
  // TEST_ASSERT(map->size(map) == 26, ERR_MAP_SIZE);

  // vector_t *keys = new_vector(vector_cemu_pchar);
  // vector_t *vals = new_vector(vector_cemu_pchar);

  // map->extract(map, keys, vals);

  // for (char c = 'a'; c <= 'z'; c++) {
  //   char cc[] = { c, '\0' };
  //   TEST_ASSERT(strcmp(*(char **)keys->at(keys, c - 'a'), cc) == 0, ERR_MAP_INSERT);
  //   TEST_ASSERT(strcmp(*(char **)vals->at(vals, c - 'a'), cc) == 0, ERR_MAP_INSERT);
  // }

  // free_vector(keys);
  // free_vector(vals);

  // free_map(map);
}

// struct pair_int {
//   int fi, se;
// };

// int pair_int_cmp(void *lhs, void *rhs)
// {
//   struct pair_int *le = (struct pair_int *)lhs;
//   struct pair_int *ri = (struct pair_int *)rhs;

//   if (le->fi == ri->fi && le->se == ri->se) return 0;
//   return le->fi > ri->fi ? 1 : -1;
// }

// void *pair_int_clone(void *val)
// {
//   struct pair_int *val_copy = (struct pair_int *)malloc(sizeof(struct pair_int));

//   val_copy->fi = ((struct pair_int *)val)->fi;
//   val_copy->se = ((struct pair_int *)val)->se;

//   return val_copy;
// }
// void pair_int_free(void *val)
// {
// }

void test_str_struct()
{
  // TEST_PRINT_FUNC();

  // map_t *map = new_map(treap_cemu_pchar, (treap_cemu_t) {
  //   pair_int_clone, pair_int_free, pair_int_cmp
  // });

  // map->insert(map, &(char *){"{1, 2}"}, &(struct pair_int){ 1, 2 });
  // map->insert(map, &(char *){"{2, 3}"}, &(struct pair_int){ 2, 3 });

  // TEST_ASSERT(((struct pair_int *)map->find(map, &(char *){"{1, 2}"}))->fi == 1, ERR_MAP_INSERT);

  // free_map(map);
}