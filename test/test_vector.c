#include "test.h"
#include "vector.h"
#include "str.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>

void test_int();
void test_vector_int();
void test_string();
void test_struct();

int main()
{
  TEST_PRINT_FILE();

  test_overall((test_t[]){
    test_int,
    test_vector_int,
    test_string,
    test_struct,
    NULL
  });
}

void test_int()
{
  TEST_PRINT_FUNC();

  vector_t *vec = new_vector(cemu(int));

  for (int i = 0; i < 10; i++) {
    vector_push_back(vec, cemu_from(int, &i));
    TEST_ASSERT(*(int *)vector_at(vec, i) == i);
  }

  for (int i = 0; i < 5; i++) {
    vector_pop_back(vec);
  }
  TEST_ASSERT(vector_size(vec) == 5);

  for (int i = 0; i < vector_size(vec); i++) {
    TEST_ASSERT(*(int *)vector_at(vec, i) == i);
  }

  vector_erase(vec, 0);
  vector_erase(vec, 0);
  TEST_ASSERT(vector_size(vec) == 3);

  for (int i = 0; i < vector_size(vec); i++) {
    TEST_ASSERT(*(int *)vector_at(vec, i) == i + 2);
  }

  delete_vector(vec);
}

void test_vector_int()
{
  TEST_PRINT_FUNC();

  vector_t *vec1 = new_vector(cemu_vector());

  for (int i = 0; i < 3; i++) {
    vector_t *vec2 = new_vector(cemu(int));
    vector_push_back(vec2, cemu_from(int, &(int){1}));
    vector_push_back(vec2, cemu_from(int, &(int){2}));
    vector_push_back(vec2, cemu_from(int, &(int){3}));
    vector_push_back(vec1, vec2);
  }

  for (int i = 0; i < 3; i++) {
    vector_t *tmp = vector_at(vec1, i);
    for (int j = 0; j < 3; j++) {
      int *val = vector_at(tmp, j);
      TEST_ASSERT(*val == j + 1);
    }
  }

  delete_vector(vec1);
}

void test_string()
{
  TEST_PRINT_FUNC();

  vector_t *vec = new_vector(cemu_string());

  vector_push_back(vec, string_from("Hello"));
  TEST_ASSERT(strcmp(string_at(vector_at(vec, 0), 0), "Hello") == 0);
  vector_push_back(vec, string_from("World"));
  TEST_ASSERT(strcmp(string_at(vector_at(vec, 1), 0), "World") == 0);

  delete_vector(vec);
}

typedef struct {
  char name[10];
  int age;
} people_t;

void test_struct() {
  TEST_PRINT_FUNC();

  vector_t *vec = new_vector(cemu(people_t));
  people_t people[] = {
    { "John", 1 },
    { "Mary", 5 },
    { "Max", 10 },
    { "Ben", 43 },
    { "Tom", 18 }
  };

  vector_push_back(vec, cemu_from(people_t, &people[0]));
  vector_push_back(vec, cemu_from(people_t, &people[1]));
  vector_push_back(vec, cemu_from(people_t, &(people_t){ "Max", 10 }));
  vector_push_back(vec, cemu_from(people_t, &(people_t){ "Ben", 43 }));
  vector_push_back(vec, cemu_from(people_t, &(people_t){ "Tom", 18 }));

  for (int i = 0; i < vector_size(vec); i++) {
    people_t p = *(people_t *)vector_at(vec, i);
    TEST_ASSERT(strcmp(p.name, people[i].name) == 0);
    TEST_ASSERT(p.age == people[i].age);
  }

  delete_vector(vec);
}