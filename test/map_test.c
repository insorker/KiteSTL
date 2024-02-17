#include "test.h"
#include "map.h"
#include "vector.h"
#include "cemu_int.h"
#include "cemu_str.h"
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
void test_struct_str();

int main()
{
  TEST_PRINT_FILE();

  TestFunction tf[] = {
    test_int_int,
    test_str_int,
    test_str_str,
    test_struct_str,
    NULL
  };

  for (int i = 0; tf[i] != NULL; i++) {
    tf[i](); printf("OK \n");
  }
}

void test_int_int()
{
  TEST_PRINT_FUNC();

  map_t *map = new_map(cemu_int(), cemu_int(), NULL);

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

  delete_map(map);
}

void test_str_int()
{
  TEST_PRINT_FUNC();

  map_t *map = new_map(cemu_str(), cemu_int(), NULL);

  map->insert(map, &(char *){"John"}, &(int){100});
  map->insert(map, &(char *){"Mary"}, &(int){98});
  map->insert(map, &(char *){"David"}, &(int){70});
  TEST_ASSERT(map->size(map) == 3, ERR_MAP_SIZE);

  TEST_ASSERT(*(int *)map->find(map, &(char *){"John"}) == 100, ERR_MAP_FIND);
  TEST_ASSERT(*(int *)map->find(map, &(char *){"Mary"}) == 98, ERR_MAP_FIND);

  TEST_ASSERT(*(int *)map->find(map, &(char *){"David"}) == 70, ERR_MAP_FIND);
  TEST_ASSERT(map->find(map, &(char *){"Tom"}) == NULL, ERR_MAP_FIND);

  map->erase(map, &(char *){"Tom"});
  map->erase(map, &(char *){"John"});
  TEST_ASSERT(map->size(map) == 2, ERR_MAP_SIZE);

  TEST_ASSERT(map->find(map, &(char *){"John"}) == NULL, ERR_MAP_ERASE);
  TEST_ASSERT(*(int *)map->find(map, &(char *){"David"}) == 70, ERR_MAP_ERASE);

  delete_map(map);
}

void test_str_str()
{
  TEST_PRINT_FUNC();

  map_t *map = new_map(cemu_str(), cemu_str(), NULL);

  for (char c = 'a'; c <= 'z'; c++) {
    char cc[] = { c, '\0' };

    map->insert(map, &(char *){cc}, &(char *){cc});
  }

  for (char c = 'a'; c <= 'z'; c++) {
    char cc[2] = { c, '\0' };

    TEST_ASSERT(
      strcmp(*(char **)map->find(map, &(char *){cc}), cc) == 0,
      ERR_MAP_INSERT
    );
  }
  TEST_ASSERT(map->size(map) == 26, ERR_MAP_SIZE);

  vector_t *keys = new_vector(cemu_str());
  vector_t *vals = new_vector(cemu_str());

  map->extract(map, keys, vals);

  for (char c = 'a'; c <= 'z'; c++) {
    char cc[] = { c, '\0' };
    TEST_ASSERT(strcmp(*(char **)keys->at(keys, c - 'a'), cc) == 0, ERR_MAP_INSERT);
    TEST_ASSERT(strcmp(*(char **)vals->at(vals, c - 'a'), cc) == 0, ERR_MAP_INSERT);
  }

  delete_vector(keys);
  delete_vector(vals);

  delete_map(map);
}

typedef struct {
  int fi, se;
} pair_int_t;

void *cemu_pair_int_copy(void *other)
{
  pair_int_t *p_other = other;
  pair_int_t *p = malloc(sizeof(pair_int_t));

  p->fi = p_other->fi;
  p->se = p_other->se;

  return p;
}

void cemu_pair_int_delete(void *self)
{
  free(self);
}

int pair_int_cmp(void *lhs, void *rhs)
{
  pair_int_t *le = (pair_int_t *)lhs;
  pair_int_t *ri = (pair_int_t *)rhs;

  if (le->fi == ri->fi && le->se == ri->se) return 0;
  return le->fi > ri->fi ? 1 : -1;
}

cemu_t cemu_pair_int()
{
  return (cemu_t){
    NULL,
    NULL, cemu_pair_int_copy, NULL, cemu_pair_int_delete
  };
}

void test_struct_str()
{
  TEST_PRINT_FUNC();

  map_t *map = new_map(cemu_pair_int(), cemu_str(), pair_int_cmp);

  map->insert(map, &(pair_int_t){ 1, 2 }, &(char *){"{1, 2}"});
  map->insert(map, &(pair_int_t){ 2, 3 }, &(char *){"{2, 3}"});

  TEST_ASSERT(strcmp(*(char **)map->find(map, &(pair_int_t){ 1, 2 }), "{1, 2}") == 0, ERR_MAP_INSERT);

  delete_map(map);
}