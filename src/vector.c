#include "vector.h"
#include <malloc.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

#define VECTOR_DEFAULT_CAPACITY 3

static int vector_size(vector_t *);
static int vector_empty(vector_t *);
static void *vector_at(vector_t *, int n);
static void vector_insert(vector_t *, int n, void *val);
static void vector_remove(vector_t *, int n);
static void vector_push_back(vector_t *, void *val);
static void vector_pop_back(vector_t *);
static void vector_clear(vector_t *);

static void vector_expand(vector_t *);
static void vector_shrink(vector_t *);


vector_t *new_vector(int size_type) {
  vector_t *vec = (vector_t *)malloc(sizeof(vector_t));

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

void free_vector(vector_t *vec) {
  free(vec->_elem);
  free(vec);
}


static int vector_size(vector_t *vec) {
  return vec->_size;
}

static int vector_empty(vector_t *vec) {
  return vec->_size == 0;
}

static void *vector_at(vector_t *vec, int n) {
  assert(0 <= n && n < vec->_size);

  return vec->_elem + vec->_size_type * n;
}

static void vector_insert(vector_t *vec, int n, void *val) {
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

static void vector_remove(vector_t *vec, int n) {
  assert(0 <= n && n < vec->_size);

  for (int i = vec->_size - 1; i > n; i--) {
    void *elem_next = vec->_elem + vec->_size_type * i;
    void *elem_prev = vec->_elem + vec->_size_type * (i - 1);

    memcpy(elem_prev, elem_next, vec->_size_type);
  }
  vec->_size -= 1;
  vec->shrink(vec);
}

static void vector_push_back(vector_t *vec, void *val) {
  vec->insert(vec, vec->_size, val);
}

static void vector_pop_back(vector_t *vec) {
  vec->remove(vec, vec->_size - 1);
}

static void vector_clear(vector_t *vec) {
  while (!vec->empty(vec)) {
    vec->pop_back(vec);
  }
}

static void vector_expand(vector_t *vec) {
  if (vec->_size < vec->_capacity) return;
  assert(vec->_capacity * 2 >= 0);

  vec->_capacity *= 2;
  vec->_elem = (void *)realloc(vec->_elem, vec->_capacity * vec->_size_type);
}

static void vector_shrink(vector_t *vec) {
  if (vec->_capacity < VECTOR_DEFAULT_CAPACITY * 2) return;
  if (vec->_capacity < vec->_size * 4) return;

  vec->_capacity /= 2;
  vec->_elem = (void *)realloc(vec->_elem, vec->_capacity * vec->_size_type);
}
