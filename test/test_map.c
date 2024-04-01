#include "test.h"
#include "map.h"
#include "vector.h"
#include "str.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>

const char *ERR_MAP_FIND = "Error -> map_find";
const char *ERR_MAP_INSERT = "Error -> map_insert";
const char *ERR_MAP_ERASE = "Error -> map_erase";
const char *ERR_MAP_SIZE = "Error -> map_size";

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
    // test_str_str,
    // test_struct_str,
    NULL
  };

  for (int i = 0; tf[i] != NULL; i++) {
    tf[i](); printf("OK \n");
  }
}

void test_int_int()
{
  TEST_PRINT_FUNC();

  map_t *map = new_map(cemu(int, {}), cemu(int, {}));

  map_insert(map, &(int){1}, &(int){1});
  map_insert(map, &(int){2}, &(int){2});

  TEST_ASSERT(*(int *)map_find(map, &(int){1}) == 1, ERR_MAP_FIND);
  TEST_ASSERT(*(int *)map_find(map, &(int){2}) == 2, ERR_MAP_FIND);
  TEST_ASSERT(map_find(map, &(int){3}) == NULL, ERR_MAP_FIND);

  map_insert(map, &(int){2}, &(int){22});

  TEST_ASSERT(*(int *)map_find(map, &(int){2}) == 22, ERR_MAP_INSERT);
  TEST_ASSERT(map_size(map) == 2, ERR_MAP_SIZE);

  map_erase(map, &(int){3});
  map_erase(map, &(int){1});

  TEST_ASSERT(map_find(map, &(int){1}) == NULL, ERR_MAP_ERASE);

  map_clear(map);
  TEST_ASSERT(map_size(map) == 0, ERR_MAP_SIZE);

  for (int i = 0; i < 100; i++) {
    map_insert(map, &(int){i}, &(int){i});
    TEST_ASSERT(map_size(map) == i + 1, ERR_MAP_SIZE);
  }

  delete_map(map);
}

void test_str_int()
{
  TEST_PRINT_FUNC();

  map_t *map = new_map(cemu_string(), cemu(int, {}));

  {
    int v = 100;
    string_t *k = new_string("John");
    map_insert(map, k, &v);
    delete_string(k);
  }
  {
    int v = 98;
    string_t *k = new_string("Mary");
    map_insert(map, k, &v);
    delete_string(k);
  }
  {
    int v = 70;
    string_t *k = new_string("David");
    map_insert(map, k, &v);
    delete_string(k);
  }

  TEST_ASSERT(map_size(map) == 3, ERR_MAP_SIZE);
  {
    string_t *k = new_string("John");
    TEST_ASSERT(*(int *)map_find(map, k) == 100, ERR_MAP_FIND);
    delete_string(k);
  }
  {
    string_t *k = new_string("Mary");
    TEST_ASSERT(*(int *)map_find(map, k) == 98, ERR_MAP_FIND);
    delete_string(k);
  }
  {
    string_t *k = new_string("David");
    TEST_ASSERT(*(int *)map_find(map, k) == 70, ERR_MAP_FIND);
    delete_string(k);
  }
  {
    string_t *k = new_string("Tom");
    TEST_ASSERT(map_find(map, k) == NULL, ERR_MAP_FIND);
    delete_string(k);
  }

  {
    string_t *k = new_string("Tom");
    map_erase(map, k);
    delete_string(k);
  }
  {
    string_t *k = new_string("John");
    map_erase(map, k);
    delete_string(k);
  }

  TEST_ASSERT(map_size(map) == 2, ERR_MAP_SIZE);
  {
    string_t *k = new_string("John");
    TEST_ASSERT(map_find(map, k) == NULL, ERR_MAP_FIND);
    delete_string(k);
  }
  {
    string_t *k = new_string("David");
    TEST_ASSERT(*(int *)map_find(map, k) == 70, ERR_MAP_FIND);
    delete_string(k);
  }

  delete_map(map);
}

// void test_str_str()
// {
//   TEST_PRINT_FUNC();

//   map_t *map = new_map(cemu_str(), cemu_str(), NULL);

//   for (char c = 'a'; c <= 'z'; c++) {
//     char cc[] = { c, '\0' };

//     map_insert(map, &(char *){cc}, &(char *){cc});
//   }

//   for (char c = 'a'; c <= 'z'; c++) {
//     char cc[2] = { c, '\0' };

//     TEST_ASSERT(
//       strcmp(*(char **)map_find(map, &(char *){cc}), cc) == 0,
//       ERR_MAP_INSERT
//     );
//   }
//   TEST_ASSERT(map_size(map) == 26, ERR_MAP_SIZE);

//   vector_t *keys = new_vector(cemu_str());
//   vector_t *vals = new_vector(cemu_str());

//   map->extract(map, keys, vals);

//   for (char c = 'a'; c <= 'z'; c++) {
//     char cc[] = { c, '\0' };
//     TEST_ASSERT(strcmp(*(char **)keys->at(keys, c - 'a'), cc) == 0, ERR_MAP_INSERT);
//     TEST_ASSERT(strcmp(*(char **)vals->at(vals, c - 'a'), cc) == 0, ERR_MAP_INSERT);
//   }

//   delete_vector(keys);
//   delete_vector(vals);

//   delete_map(map);
// }

// typedef struct {
//   int fi, se;
// } pair_int_t;

// void *cemu_pair_int_copy(void *other)
// {
//   pair_int_t *p_other = other;
//   pair_int_t *p = malloc(sizeof(pair_int_t));

//   p->fi = p_other->fi;
//   p->se = p_other->se;

//   return p;
// }

// void cemu_pair_int_delete(void *self)
// {
//   free(self);
// }

// int pair_int_cmp(void *lhs, void *rhs)
// {
//   pair_int_t *le = (pair_int_t *)lhs;
//   pair_int_t *ri = (pair_int_t *)rhs;

//   if (le->fi == ri->fi && le->se == ri->se) return 0;
//   return le->fi > ri->fi ? 1 : -1;
// }

// cemu_t cemu_pair_int()
// {
//   return (cemu_t){
//     NULL,
//     NULL, cemu_pair_int_copy, NULL, cemu_pair_int_delete
//   };
// }

// void test_struct_str()
// {
//   TEST_PRINT_FUNC();

//   map_t *map = new_map(cemu_pair_int(), cemu_str(), pair_int_cmp);

//   map_insert(map, &(pair_int_t){ 1, 2 }, &(char *){"{1, 2}"});
//   map_insert(map, &(pair_int_t){ 2, 3 }, &(char *){"{2, 3}"});

//   TEST_ASSERT(strcmp(*(char **)map_find(map, &(pair_int_t){ 1, 2 }), "{1, 2}") == 0, ERR_MAP_INSERT);

//   delete_map(map);
// }