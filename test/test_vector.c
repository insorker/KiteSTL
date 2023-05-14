#include "test.h"
#include "vector.h"
#include <stdio.h>

void test_int();
void test_struct();
void test_string();
void test_2d_array();
void test_value_or_reference();

int main() {
  TEST_PRINT_FILE();

  TestFunction tf[] = {
    test_int,
    test_struct,
    test_string,
    test_2d_array,
    test_value_or_reference,
    NULL
  };

  for (int i = 0; tf[i] != NULL; i++) {
    tf[i]();
    printf("\n");
  }
}

void test_int() {
  TEST_PRINT_FUNC();

  Vector *vec = new_vector(sizeof(int));

  vec->push_back(vec, &(int){1});
  vec->push_back(vec, &(int){2});
  vec->push_back(vec, &(int){3});
  vec->push_back(vec, &(int){4});
  vec->push_back(vec, &(int){5});
  printf("push_back 1 2 3 4 5: \n\t");
  for (int i = 0; i < vec->size(vec); i++) {
    printf("%d ", *(int *)vec->at(vec, i));
  }
  printf("\n");

  printf("pop_back: \n\t");
  vec->pop_back(vec);
  for (int i = 0; i < vec->size(vec); i++) {
    printf("%d ", *(int *)vec->at(vec, i));
  }
  printf("\n");

  printf("insert 0 -1: \n\t");
  vec->insert(vec, 0, &(int){-1});
  for (int i = 0; i < vec->size(vec); i++) {
    printf("%d ", *(int *)vec->at(vec, i));
  }
  printf("\n");

  printf("remove 3: \n\t");
  vec->remove(vec, 3);
  for (int i = 0; i < vec->size(vec); i++) {
    printf("%d ", *(int *)vec->at(vec, i));
  }
  printf("\n");

  printf("clear: \n\t");
  vec->clear(vec);
  for (int i = 0; i < vec->size(vec); i++) {
    printf("%d ", *(int *)vec->at(vec, i));
  }
  printf("\n");

  free_vector(vec);
}

void test_struct() {
  TEST_PRINT_FUNC();

  typedef struct People {
    char name[10];
    int age;
  } People;

  Vector *vec = new_vector(sizeof(People));
  People kate = { "kate", 1 };
  People leo = { "Leo", 5 };

  vec->push_back(vec, &(People){ "Max", 10 });
  vec->push_back(vec, &(People){ "Ben", 43 });
  vec->push_back(vec, &(People){ "Tom", 18 });
  vec->push_back(vec, &kate);
  vec->push_back(vec, &leo);

  for (int i = 0; i < vec->size(vec); i++) {
    People people = *(People *)vec->at(vec, i);
    printf("name: %s\tage:%d\n", people.name, people.age);
  }

  free_vector(vec);
}

void test_string() {
  TEST_PRINT_FUNC();

  Vector *vec = new_vector(sizeof(char *));

  vec->push_back(vec, &(char *){"Hello"});
  vec->push_back(vec, &(char *){"World"});

  for (int i = 0; i < vec->size(vec); i++) {
    char *str = *(char * *)vec->at(vec, i);
    printf("%s ", str);
  }

  free_vector(vec);
}

void test_2d_array() {
  TEST_PRINT_FUNC();

  Vector *vec = new_vector(sizeof(double **));
  double a[2][2] = {
    { 1, 2 },
    { 3, 4 }
  };
  double b[3][3] = {
    { 1, 3, 5 },
    { 1, 4, 9 },
    { 1, 1, 2 }
  };

  vec->push_back(vec, &(double(*)[2]){a});
  vec->push_back(vec, &(void *){b});

  double (*arr_a)[2] = *(double(**)[2])vec->at(vec, 0);
  for (int j = 0; j < 2; j++) {
    for (int k = 0; k < 2; k++) {
      printf("%f ", arr_a[j][k]);
    }
    printf("\n");
  }
  double (*arr_b)[3] = *(void * *)vec->at(vec, 1);
  for (int j = 0; j < 3; j++) {
    for (int k = 0; k < 3; k++) {
      printf("%f ", arr_b[j][k]);
    }
    printf("\n");
  }

  free_vector(vec);
}

void test_value_or_reference() {
  TEST_PRINT_FUNC();

  int a = 0;
  Vector *vec_int = new_vector(sizeof(int));
  vec_int->push_back(vec_int, &a);
  printf("int: \n");
  printf("%d\n", *(int *)vec_int->at(vec_int, 0));
  printf("origin address: %p\n", &a);
  printf("vector address: %p\n", (int *)vec_int->at(vec_int, 0));
  free_vector(vec_int);

  printf("\n");

  char *c = "Hello";
  Vector *vec_str = new_vector(sizeof(char *));
  vec_str->push_back(vec_str, &c);
  printf("char *: \n");
  printf("%s\n", *(char **)vec_str->at(vec_str, 0));
  printf("origin address: %p\n", &c);
  printf("vector address: %p\n", (char **)vec_str->at(vec_str, 0));
  printf("string origin address: %p\n", c);
  printf("string vector address: %p\n", *(char **)vec_str->at(vec_str, 0));
  free_vector(vec_str);
}
