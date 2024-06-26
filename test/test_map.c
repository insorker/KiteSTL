#include "test.h"
#include "map.h"
#include "vector.h"
#include "str.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>

void test_int_int();
void test_str_int();
void test_str_str();
void test_struct_str();

int main()
{
  TEST_PRINT_FILE();

  test_overall((test_t[]){
    test_int_int,
    test_str_int,
    test_str_str,
    test_struct_str,
    NULL
  });
}

void test_int_int()
{
  TEST_PRINT_FUNC();

  map_t *map = new_map(cemu(int), cemu(int));

  map_insert(map, cemu_from(int, &(int){1}), cemu_from(int, &(int){1}));
  map_insert(map, cemu_from(int, &(int){2}), cemu_from(int, &(int){2}));

  TEST_ASSERT(*(int *)map_find(map, cemu_from(int, &(int){1})) == 1);
  TEST_ASSERT(*(int *)map_find(map, cemu_from(int, &(int){2})) == 2);
  TEST_ASSERT(map_find(map, cemu_from(int, &(int){3})) == NULL);

  map_insert(map, cemu_from(int, &(int){2}), cemu_from(int, &(int){22}));

  TEST_ASSERT(*(int *)map_find(map, cemu_from(int, &(int){2})) == 22);
  TEST_ASSERT(map_size(map) == 2);

  map_erase(map, cemu_from(int, &(int){3}));
  map_erase(map, cemu_from(int, &(int){1}));

  TEST_ASSERT(map_find(map, cemu_from(int, &(int){1})) == NULL);

  map_clear(map);
  TEST_ASSERT(map_size(map) == 0);

  for (int i = 0; i < 100; i++) {
    map_insert(map, cemu_from(int, &i), cemu_from(int, &i));
    TEST_ASSERT(map_size(map) == i + 1);
  }

  delete_map(map);
}

void test_str_int()
{
  TEST_PRINT_FUNC();

  map_t *map = new_map(cemu_string(), cemu(int));

  map_insert(map, string_from("John"), cemu_from(int, &(int){100}));
  map_insert(map, string_from("Mary"), cemu_from(int, &(int){98}));
  map_insert(map, string_from("David"), cemu_from(int, &(int){70}));

  TEST_ASSERT(map_size(map) == 3);
  TEST_ASSERT(*(int *)map_find(map, string_from("John")) == 100);
  TEST_ASSERT(*(int *)map_find(map, string_from("Mary")) == 98);
  TEST_ASSERT(*(int *)map_find(map, string_from("David")) == 70);
  TEST_ASSERT(map_find(map, string_from("Tom")) == NULL);

  map_erase(map, string_from("Tom"));
  map_erase(map, string_from("John"));

  TEST_ASSERT(map_size(map) == 2);
  TEST_ASSERT(map_find(map, string_from("John")) == NULL);
  TEST_ASSERT(*(int *)map_find(map, string_from("David")) == 70);

  delete_map(map);
}

void test_str_str()
{
  TEST_PRINT_FUNC();

  map_t *map = new_map(cemu_string(), cemu_string());

  for (char c = 'a'; c <= 'z'; c++) {
    char cc[] = { c, '\0' };

    map_insert(map, string_from(cc), string_from(cc));
  }

  for (char c = 'a'; c <= 'z'; c++) {
    char cc[2] = { c, '\0' };

    TEST_ASSERT(
      strcmp(string_at(map_find(map, string_from(cc)), 0), cc) == 0
    );
  }
  TEST_ASSERT(map_size(map) == 26);

  vector_t *keys = new_vector(cemu_string());
  vector_t *vals = new_vector(cemu_string());

  map_extract(map, keys, vals);

  for (char c = 'a'; c <= 'z'; c++) {
    char cc[] = { c, '\0' };
    TEST_ASSERT(strcmp(string_at(vector_at(keys, c - 'a'), 0), cc) == 0);
    TEST_ASSERT(strcmp(string_at(vector_at(vals, c - 'a'), 0), cc) == 0);
  }

  delete_vector(keys);
  delete_vector(vals);

  delete_map(map);
}

typedef struct {
  int fi, se;
} pair_t;

int cemu_pair_cmp(cemu_data_t data, const void *lhs, const void *rhs)
{
  pair_t *le = (pair_t *)lhs;
  pair_t *ri = (pair_t *)rhs;

  if (le->fi == ri->fi && le->se == ri->se) return 0;
  return le->fi > ri->fi ? 1 : -1;
}

cemu_t cemu_pair()
{
  cemu_impl_t impl = {
    NULL, NULL, NULL, NULL, NULL, NULL, cemu_pair_cmp
  };
  return proto_cemu({sizeof(pair_t)}, impl, {});
}

void test_struct_str()
{
  TEST_PRINT_FUNC();

  map_t *map = new_map(cemu_pair(), cemu_string());

  map_insert(map, cemu_from(pair_t, &(pair_t){ 1, 2 }), string_from("{1, 2}"));
  map_insert(map, cemu_from(pair_t, &(pair_t){ 2, 3 }), string_from("{2, 3}"));

  TEST_ASSERT(strcmp(string_at(map_find(map, cemu_from(pair_t, &(pair_t){ 1, 2 })), 0), "{1, 2}") == 0);

  delete_map(map);
}