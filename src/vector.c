#include "vector.h"
#include <malloc.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

#define VECTOR_DEFAULT_CAPACITY 3

static int   cemu_vector_size();
static void *cemu_vector_new(void *arg);
static void  cemu_vector_delete(void *self);
cemu_t cemu_vector()
{
  return (cemu_t){
    cemu_vector_size, cemu_vector_new, NULL, NULL, cemu_vector_delete
  };
}

static size_t vector_size(vector_t *);
static bool vector_empty(vector_t *);
static void *vector_at(vector_t *, size_t n);
static void vector_insert(vector_t *, size_t n, void *val);
static void vector_erase(vector_t *, size_t n);
static void vector_push_back(vector_t *, void *val);
static void vector_pop_back(vector_t *);
static void vector_clear(vector_t *);

static void vector_expand(vector_t *);
static void vector_shrink(vector_t *);

static int cemu_vector_size()
{
  return sizeof(vector_t);
}

static void *cemu_vector_new(void *arg)
{
  cemu_t cemu = *(cemu_t *)arg;
  vector_t *vec = malloc(sizeof(vector_t));

  vec->size = vector_size;
  vec->empty = vector_empty;
  vec->at = vector_at;
  vec->insert = vector_insert;
  vec->erase = vector_erase;
  vec->push_back = vector_push_back;
  vec->pop_back = vector_pop_back;
  vec->clear = vector_clear;

  vec->expand = vector_expand;
  vec->shrink = vector_shrink;

  vec->_size = 0;
  vec->_capacity = VECTOR_DEFAULT_CAPACITY;
  vec->_tsize = cemu.size();
  vec->_elem = malloc(vec->_capacity * vec->_tsize);

  vec->_cemu_elem = cemu;

  return vec;
}

static void cemu_vector_delete(void *self)
{
  vector_t *vec = self;

  for (size_t i = 0; i < vec->_size; i++) {
    void *elem = vec->_elem + vec->_tsize * i;
    vec->_cemu_elem.dtor(elem);
  }
  free(vec->_elem);
  free(vec);
}

vector_t *new_vector(cemu_t cemu)
{
  return cemu_vector_new(&cemu);
}

void delete_vector(vector_t *vec)
{
  cemu_vector_delete(vec);
}

static size_t vector_size(vector_t *vec)
{
  return vec->_size;
}

static bool vector_empty(vector_t *vec)
{
  return vec->_size == 0;
}

static void *vector_at(vector_t *vec, size_t n)
{
  assert(0 <= n && n < vec->_size);

  return vec->_elem + vec->_tsize * n;
}

static void vector_insert(vector_t *vec, size_t n, void *val)
{
  assert(0 <= n && n <= vec->_size);

  vec->expand(vec);
  for (size_t i = vec->_size; i > n; i--) {
    void *elem_next = vec->_elem + vec->_tsize * i;
    void *elem_prev = vec->_elem + vec->_tsize * (i - 1);

    memcpy(elem_next, elem_prev, vec->_tsize);
  }

  void *dest = vec->_elem + vec->_tsize * n;
  void *src = val;
  vec->_cemu_elem.assign(dest, src, vec->_tsize);

  vec->_size += 1;
}

static void vector_erase(vector_t *vec, size_t n)
{
  assert(0 <= n && n < vec->_size);

  for (size_t i = n + 1; i < vec->_size; i++) {
    void *elem_next = vec->_elem + vec->_tsize * i;
    void *elem_prev = vec->_elem + vec->_tsize * (i - 1);

    memcpy(elem_prev, elem_next, vec->_tsize);
  }

  vec->_size -= 1;
  vec->shrink(vec);
}

static void vector_push_back(vector_t *vec, void *val)
{
  vec->insert(vec, vec->_size, val);
}

static void vector_pop_back(vector_t *vec)
{
  vec->erase(vec, vec->_size - 1);
}

static void vector_clear(vector_t *vec)
{
  while (!vec->empty(vec)) {
    vec->pop_back(vec);
  }
}

static void vector_expand(vector_t *vec)
{
  if (vec->_size < vec->_capacity) return;
  assert(vec->_capacity * 2 >= vec->_capacity);

  vec->_capacity *= 2;
  vec->_elem = realloc(vec->_elem, vec->_capacity * vec->_tsize);
}

static void vector_shrink(vector_t *vec)
{
  if (vec->_capacity == VECTOR_DEFAULT_CAPACITY) return;
  if (vec->_capacity < vec->_size * 2) return;

  vec->_capacity /= 2;
  vec->_elem = realloc(vec->_elem, vec->_capacity * vec->_tsize);
}