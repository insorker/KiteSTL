#include "test.h"
#include "vector.h"
#include "str.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>

const char *ERR_VECTOR_FIND = "Error -> vector->find";
const char *ERR_VECTOR_INSERT = "Error -> vector->insert";
const char *ERR_VECTOR_ERASE = "Error -> vector->erase";
const char *ERR_VECTOR_SIZE = "Error -> vector->size";

void test_int();
void test_vector_int();
void test_string();
void test_struct();

int main()
{
  TEST_PRINT_FILE();

  TestFunction tf[] = {
    test_int,
    test_vector_int,
    test_string,
    test_struct,
    NULL
  };

  for (int i = 0; tf[i] != NULL; i++) {
    tf[i](); printf("OK \n");
  }
}

void test_int()
{
  TEST_PRINT_FUNC();

  vector_t *vec = new_vector(cemu(int, {}));

  for (int i = 0; i < 10; i++) {
    vector_push_back(vec, &i);
    TEST_ASSERT(*(int *)vector_at(vec, i) == i, ERR_VECTOR_FIND);
  }

  for (int i = 0; i < 5; i++) {
    vector_pop_back(vec);
  }
  TEST_ASSERT(vector_size(vec) == 5, ERR_VECTOR_ERASE);

  for (int i = 0; i < vector_size(vec); i++) {
    TEST_ASSERT(*(int *)vector_at(vec, i) == i, ERR_VECTOR_FIND);
  }

  vector_erase(vec, 0);
  vector_erase(vec, 0);
  TEST_ASSERT(vector_size(vec) == 3, ERR_VECTOR_ERASE);

  for (int i = 0; i < vector_size(vec); i++) {
    TEST_ASSERT(*(int *)vector_at(vec, i) == i + 2, ERR_VECTOR_FIND);
  }

  delete_vector(vec);
}

void test_vector_int()
{
  TEST_PRINT_FUNC();

  vector_t *vec1 = new_vector(cemu_vector());

  {
    vector_t *vec2 = new_vector(cemu(int, {}));
    vector_push_back(vec2, cemu_make(int, 1));
    vector_push_back(vec2, cemu_make(int, 2));
    vector_push_back(vec2, cemu_make(int, 3));

    vector_push_back(vec1, vec2);

    delete_vector(vec2);
  }

  {
    vector_t *vec2 = new_vector(cemu(int, {}));
    vector_push_back(vec2, cemu_make(int, 1));
    vector_push_back(vec2, cemu_make(int, 2));
    vector_push_back(vec2, cemu_make(int, 3));

    vector_push_back(vec1, vec2);

    delete_vector(vec2);
  }

  {
    vector_t *vec2 = new_vector(cemu(int, {}));
    vector_push_back(vec2, cemu_make(int, 1));
    vector_push_back(vec2, cemu_make(int, 2));
    vector_push_back(vec2, cemu_make(int, 3));

    vector_push_back(vec1, vec2);

    delete_vector(vec2);
  }

  for (int i = 0; i < 3; i++) {
    vector_t *tmp = vector_at(vec1, i);
    for (int j = 0; j < 3; j++) {
      int *val = vector_at(tmp, j);
      TEST_ASSERT(*val == j + 1, ERR_VECTOR_FIND);
    }
  }

  delete_vector(vec1);
}

void test_string()
{
  TEST_PRINT_FUNC();

  vector_t *vec = new_vector(cemu_string());

  {
    string_t str = string("Hello");
    vector_push_back(vec, &str);
    _string(str);
  }
  TEST_ASSERT(strcmp(string_at(vector_at(vec, 0), 0), "Hello") == 0, ERR_VECTOR_INSERT);
  {
    string_t str = string("World");
    vector_push_back(vec, &str);
    _string(str);
  }
  TEST_ASSERT(strcmp(string_at(vector_at(vec, 1), 0), "World") == 0, ERR_VECTOR_INSERT);

  delete_vector(vec);
}

typedef struct {
  char name[10];
  int age;
} people_t;

void test_struct() {
  TEST_PRINT_FUNC();

  vector_t *vec = new_vector(cemu(people_t, {}));
  people_t people[] = {
    { "John", 1 },
    { "Mary", 5 },
    { "Max", 10 },
    { "Ben", 43 },
    { "Tom", 18 }
  };

  vector_push_back(vec, &people[0]);
  vector_push_back(vec, &people[1]);
  vector_push_back(vec, &(people_t){ "Max", 10 });
  vector_push_back(vec, &(people_t){ "Ben", 43 });
  vector_push_back(vec, &(people_t){ "Tom", 18 });

  for (int i = 0; i < vector_size(vec); i++) {
    people_t p = *(people_t *)vector_at(vec, i);
    TEST_ASSERT(strcmp(p.name, people[i].name) == 0, ERR_VECTOR_INSERT);
    TEST_ASSERT(p.age == people[i].age, ERR_VECTOR_INSERT);
  }

  delete_vector(vec);
}