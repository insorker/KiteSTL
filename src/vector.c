#include "vector.h"
#include <malloc.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

#define VECTOR_DEFAULT_CAPACITY 3

int vector_size(Vector *);
int vector_empty(Vector *);
void *vector_at(Vector *, int n);
void vector_insert(Vector *, int n, void *val);
void vector_remove(Vector *, int n);
void vector_push_back(Vector *, void *val);
void vector_pop_back(Vector *);
void vector_clear(Vector *);

void vector_expand(Vector *);
void vector_shrink(Vector *);


Vector *new_vector(int size_type) {
  Vector *vec = (Vector *)malloc(sizeof(Vector));

  vec->size = vector_size;
  vec->empty = vector_empty;
  vec->at = vector_at;
  vec->insert = vector_insert;
  vec->remove = vector_remove;
  vec->push_back = vector_push_back;
  vec->pop_back = vector_pop_back;
  vec->clear = vector_clear;

  vec->expand = vector_expand;
  vec->shrink = vector_shrink;

  vec->_size = 0;
  vec->_capacity = VECTOR_DEFAULT_CAPACITY;
  vec->_size_type = size_type;
  vec->_elem = (void *)malloc(vec->_capacity * vec->_size_type);

  return vec;
}

void free_vector(Vector *vec) {
  free(vec->_elem);
  free(vec);
}


int vector_size(Vector *vec) {
  return vec->_size;
}

int vector_empty(Vector *vec) {
  return vec->_size == 0;
}

void *vector_at(Vector *vec, int n) {
  assert(0 <= n && n < vec->_size);

  return vec->_elem + vec->_size_type * n;
}

void vector_insert(Vector *vec, int n, void *val) {
  assert(0 <= n && n <= vec->_size);

  vec->expand(vec);
  for (int i = vec->_size; i > n; i--) {
    void *elem_next = vec->_elem + vec->_size_type * i;
    void *elem_prev = vec->_elem + vec->_size_type * (i - 1);

    memcpy(elem_next, elem_prev, vec->_size_type);
  }
  memcpy(vec->_elem + vec->_size_type * n, val, vec->_size_type);
  vec->_size += 1;
}

void vector_remove(Vector *vec, int n) {
  assert(0 <= n && n < vec->_size);

  for (int i = vec->_size - 1; i > n; i--) {
    void *elem_next = vec->_elem + vec->_size_type * i;
    void *elem_prev = vec->_elem + vec->_size_type * (i - 1);

    memcpy(elem_prev, elem_next, vec->_size_type);
  }
  vec->_size -= 1;
  vec->shrink(vec);
}

void vector_push_back(Vector *vec, void *val) {
  vec->insert(vec, vec->_size, val);
}

void vector_pop_back(Vector *vec) {
  vec->remove(vec, vec->_size - 1);
}

void vector_clear(Vector *vec) {
  while (!vec->empty(vec)) {
    vec->pop_back(vec);
  }
}

void vector_expand(Vector *vec) {
  if (vec->_size < vec->_capacity) return;
  assert(vec->_capacity * 2 >= 0);

  vec->_capacity *= 2;
  vec->_elem = (void *)realloc(vec->_elem, vec->_capacity * vec->_size_type);
}

void vector_shrink(Vector *vec) {
  if (vec->_capacity < VECTOR_DEFAULT_CAPACITY * 2) return;
  if (vec->_capacity < vec->_size * 4) return;

  vec->_capacity /= 2;
  vec->_elem = (void *)realloc(vec->_elem, vec->_capacity * vec->_size_type);
}
