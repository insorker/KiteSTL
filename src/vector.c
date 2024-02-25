#include "vector.h"
#include <malloc.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

#define VECTOR_DEFAULT_CAPACITY 3

static void vector_expand(vector_t *);
static void vector_shrink(vector_t *);

/**
 * ====================
 * cemu
 * ==================== 
 */

cemu_t cemu_vector()
{
  return (cemu_t){
    cemu_vector_size,
    cemu_vector_new, cemu_vector_copy, cemu_vector_dtor, cemu_vector_delete, cemu_vector_op_assign,
    NULL
  };
}

int cemu_vector_size()
{
  return sizeof(vector_t);
}

void *cemu_vector_new(void *arg)
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

void *cemu_vector_copy(void *other)
{
  vector_t *vec_other = other;
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

  vec->_size = vec_other->_size;
  vec->_capacity = vec_other->_capacity;
  vec->_tsize = vec_other->_tsize;
  vec->_elem = malloc(vec->_capacity * vec->_tsize);

  vec->_cemu_elem = vec_other->_cemu_elem;

  for (size_t i = 0; i < vec->_size; i++) {
    void *elem = vec->_elem + vec->_tsize * i;
    void *elem_other = vec_other->_elem + vec_other->_tsize * i;

    memcpy(elem, elem_other, vec->_cemu_elem.size());
  }

  return vec;
}

void cemu_vector_dtor(void *self)
{
  vector_t *vec = self;

  for (size_t i = 0; i < vec->_size; i++) {
    void *elem = vec->_elem + vec->_tsize * i;
    vec->_cemu_elem.dtor(elem);
  }
  free(vec->_elem);
}

void cemu_vector_delete(void *self)
{
  cemu_vector_dtor(self);
  free(self);
}

void cemu_vector_op_assign(void *dest, void *src)
{
  void *copy = cemu_vector_copy(src);
  memcpy(dest, copy, cemu_vector_size());
  free(copy);
}

/**
 * ====================
 * new / delete
 * ==================== 
 */

vector_t *new_vector(cemu_t cemu)
{
  return cemu_vector_new(&cemu);
}

void delete_vector(vector_t *vec)
{
  cemu_vector_delete(vec);
}

/**
 * ====================
 * api
 * ==================== 
 */

size_t vector_size(vector_t *vec)
{
  return vec->_size;
}

bool vector_empty(vector_t *vec)
{
  return vec->_size == 0;
}

void *vector_at(vector_t *vec, size_t n)
{
  if (n >= vec->_size) {
    return NULL;
  }

  return vec->_elem + vec->_tsize * n;
}

void vector_insert(vector_t *vec, size_t n, void *val)
{
  if (n > vec->_size) {
    return;
  }

  vec->expand(vec);
  for (size_t i = vec->_size; i > n; i--) {
    void *elem_next = vec->_elem + vec->_tsize * i;
    void *elem_prev = vec->_elem + vec->_tsize * (i - 1);

    memcpy(elem_next, elem_prev, vec->_tsize);
  }

  void *dest = vec->_elem + vec->_tsize * n;
  void *src = val;
  vec->_cemu_elem.assign(dest, src);

  vec->_size += 1;
}

void vector_erase(vector_t *vec, size_t n)
{
  if (n > vec->_size) {
    return;
  }

  for (size_t i = n + 1; i < vec->_size; i++) {
    void *elem_next = vec->_elem + vec->_tsize * i;
    void *elem_prev = vec->_elem + vec->_tsize * (i - 1);

    memcpy(elem_prev, elem_next, vec->_tsize);
  }

  vec->_size -= 1;
  vec->shrink(vec);
}

void vector_push_back(vector_t *vec, void *val)
{
  vec->insert(vec, vec->_size, val);
}

void vector_pop_back(vector_t *vec)
{
  vec->erase(vec, vec->_size - 1);
}

void vector_clear(vector_t *vec)
{
  while (!vec->empty(vec)) {
    vec->pop_back(vec);
  }
}

static void vector_expand(vector_t *vec)
{
  if (vec->_size < vec->_capacity) return;
  // assert(vec->_capacity * 2 >= vec->_capacity);

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