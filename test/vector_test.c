#include "test.h"
#include "vector.h"
#include "cemu_int.h"
#include "cemu_str.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>

const char *ERR_VECTOR_FIND = "Error -> vector->find";
const char *ERR_VECTOR_INSERT = "Error -> vector->insert";
const char *ERR_VECTOR_ERASE = "Error -> vector->erase";
const char *ERR_VECTOR_SIZE = "Error -> vector->size";

void test_int();
void test_vector_int();
void test_str();
void test_struct();

int main() {
  TEST_PRINT_FILE();

  TestFunction tf[] = {
    test_int,
    test_vector_int,
    test_str,
    test_struct,
    NULL
  };

  for (int i = 0; tf[i] != NULL; i++) {
    tf[i](); printf("OK \n");
  }
}

void test_int() {
  TEST_PRINT_FUNC();

  vector_t *vec = new_vector(cemu_int());

  for (int i = 0; i < 10; i++) {
    vec->push_back(vec, &i);
    TEST_ASSERT(*(int *)vec->at(vec, i) == i, ERR_VECTOR_FIND);
  }

  for (int i = 0; i < 5; i++) {
    vec->pop_back(vec);
  }
  TEST_ASSERT(vec->size(vec) == 5, ERR_VECTOR_ERASE);

  for (int i = 0; i < vec->size(vec); i++) {
    TEST_ASSERT(*(int *)vec->at(vec, i) == i, ERR_VECTOR_FIND);
  }

  vec->erase(vec, 0);
  vec->erase(vec, 0);
  TEST_ASSERT(vec->size(vec) == 3, ERR_VECTOR_ERASE);

  for (int i = 0; i < vec->size(vec); i++) {
    TEST_ASSERT(*(int *)vec->at(vec, i) == i + 2, ERR_VECTOR_FIND);
  }

  delete_vector(vec);
}

void test_vector_int()
{
  TEST_PRINT_FUNC();

  vector_t *vec1 = new_vector(cemu_vector());

  {
    vector_t *vec2 = new_vector(cemu_int());
    vec2->push_back(vec2, cemu_make(int, 1));
    vec2->push_back(vec2, cemu_make(int, 2));
    vec2->push_back(vec2, cemu_make(int, 3));

    vec1->push_back(vec1, vec2);

    delete_vector(vec2);
  }

  {
    vector_t *vec2 = new_vector(cemu_int());
    vec2->push_back(vec2, cemu_make(int, 4));
    vec2->push_back(vec2, cemu_make(int, 5));
    vec2->push_back(vec2, cemu_make(int, 6));

    vec1->push_back(vec1, vec2);

    delete_vector(vec2);
  }

  {
    vector_t *vec2 = new_vector(cemu_int());
    vec2->push_back(vec2, cemu_make(int, 7));
    vec2->push_back(vec2, cemu_make(int, 8));
    vec2->push_back(vec2, cemu_make(int, 9));

    vec1->push_back(vec1, vec2);

    delete_vector(vec2);
  }

  for (int i = 0; i < 3; i++) {
    vector_t *tmp = vec1->at(vec1, i);
    for (int j = 0; j < 3; j++) {
      int *val = tmp->at(tmp, j);
      TEST_ASSERT(*val == i * 3 + j + 1, ERR_VECTOR_FIND);
    }
  }

  delete_vector(vec1);
}

void test_str() {
  TEST_PRINT_FUNC();

  vector_t *vec = new_vector(cemu_str());

  vec->push_back(vec, cemu_make(char *, "Hello"));
  TEST_ASSERT(strcmp(cemu_get(char *, vec->at(vec, 0)), "Hello") == 0, ERR_VECTOR_INSERT);
  vec->push_back(vec, &(char *){"World"});
  TEST_ASSERT(strcmp(*(char **)vec->at(vec, 1), "World") == 0, ERR_VECTOR_INSERT);

  delete_vector(vec);
}

typedef struct {
  char name[10];
  int age;
} people_t;

int cemu_people_size()
{
  return sizeof(people_t);
}

void *cemu_people_copy(void *other)
{
  people_t *p_other = other;
  people_t *p = malloc(sizeof(people_t));

  for (int i = 0; i < 10; i++) {
    p->name[i] = p_other->name[i];
  }
  p->age = p_other->age;

  return p;
}

void cemu_people_dtor(void *self)
{
  
}

void cemu_people_op_assign(void *dest, void *src)
{
  void *copy = cemu_people_copy(src);
  memcpy(dest, copy, cemu_people_size());
  free(copy);
}

cemu_t cemu_people()
{
  return (cemu_t){
    cemu_people_size,
    NULL, cemu_people_copy, cemu_people_dtor, NULL, cemu_people_op_assign
  };
}

void test_struct() {
  TEST_PRINT_FUNC();

  vector_t *vec = new_vector(cemu_people());
  people_t people[] = {
    { "John", 1 },
    { "Mary", 5 },
    { "Max", 10 },
    { "Ben", 43 },
    { "Tom", 18 }
  };

  vec->push_back(vec, &people[0]);
  vec->push_back(vec, &people[1]);
  vec->push_back(vec, &(people_t){ "Max", 10 });
  vec->push_back(vec, &(people_t){ "Ben", 43 });
  vec->push_back(vec, &(people_t){ "Tom", 18 });

  for (int i = 0; i < vec->size(vec); i++) {
    people_t p = *(people_t *)vec->at(vec, i);
    TEST_ASSERT(strcmp(p.name, people[i].name) == 0, ERR_VECTOR_INSERT);
    TEST_ASSERT(p.age == people[i].age, ERR_VECTOR_INSERT);
  }

  delete_vector(vec);
}