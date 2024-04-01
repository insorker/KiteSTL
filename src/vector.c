#include "vector.h"
#include <malloc.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

#define VECTOR_DEFAULT_CAPACITY 3

static void vector_expand(vector_t *self);
static void vector_shrink(vector_t *self);

/**
 * ====================
 * cemu
 * ==================== 
 */

static const cemu_data_t cemu_vector_data = { sizeof(vector_t) };

static void cemu_vector_dtor(cemu_data_t data, void *self)
{
  if (self == NULL) {
    return;
  }

  vector_t *vec = self;

  for (int i = 0; i < vec->_size; i++) {
    void *elem = vec->_elems + vec->_elem_size * i;
    cemu_impl(vec->_cemu_elem, dtor, elem);
  }
  free(vec->_elems);
}

static void cemu_vector_delete(cemu_data_t data, void *self)
{
  if (self == NULL) {
    return;
  }

  cemu_vector_dtor(data, self);
  free(self);
}


static void cemu_vector_assign(cemu_data_t data, void *dest, const void *src)
{
  if (dest == NULL || src == NULL) {
    return;
  }

  vector_t *vec_dest = dest;
  const vector_t *vec_src = src;
  int elem_size = vec_src->_elem_size;

  cemu_vector_dtor(data, dest);
  memcpy(vec_dest, vec_src, sizeof(vector_t));

  vec_dest->_elems = malloc(vec_dest->_capacity * elem_size);
  for (int i = 0; i < vec_dest->_size; i++) {
    void *elem_dest = vec_dest->_elems + elem_size * i;
    void *elem_src = vec_src->_elems + elem_size * i;

    memcpy(elem_dest, elem_src, elem_size);
  }
}

static void *cemu_vector_copy(cemu_data_t data, const void *src)
{
  if (src == NULL) {
    return NULL;
  }

  vector_t *vec_dest = malloc(sizeof(vector_t));
  const vector_t *vec_src = src;
  int elem_size = vec_src->_elem_size;

  memcpy(vec_dest, vec_src, sizeof(vector_t));

  vec_dest->_elems = malloc(vec_dest->_capacity * elem_size);
  for (int i = 0; i < vec_dest->_size; i++) {
    void *elem_dest = vec_dest->_elems + elem_size * i;
    void *elem_src = vec_src->_elems + elem_size * i;

    memcpy(elem_dest, elem_src, elem_size);
  }

  return vec_dest;
}

static int cemu_vector_cmp(cemu_data_t data, const void *lhs, const void *rhs)
{
  const vector_t *vec_lhs = lhs;
  const vector_t *vec_rhs = rhs;
  cemu_t cemu_elem = vec_lhs->_cemu_elem;

  if (vec_lhs->_size != vec_rhs->_size) {
    return vec_lhs->_size > vec_rhs->_size;
  }
  
  int size = vec_lhs->_size;
  int elem_size = vec_lhs->_elem_size;
  for (int i = 0; i < size * elem_size; i += elem_size) {
    void *elem_lhs = vec_lhs->_elems + i;
    void *elem_rhs = vec_lhs->_elems + i;
    int result = cemu_impl(cemu_elem, cmp, elem_lhs, elem_rhs);

    if (result != 0) {
      return result;
    }
  }

  return 0;
}

cemu_t cemu_vector()
{
  cemu_impl_t impl = {
    NULL,
    NULL,
    cemu_vector_dtor,
    cemu_vector_delete,
    cemu_vector_assign,
    cemu_vector_copy,
    cemu_vector_cmp
  };

  return cemu(vector_t, impl);
}

/**
 * ====================
 * new / delete
 * ==================== 
 */

vector_t *new_vector(cemu_t cemu_elem)
{
  vector_t *vec = malloc(sizeof(vector_t));

  vec->_size = 0;
  vec->_elem_size = cemu_impl(cemu_elem, size);
  vec->_capacity = VECTOR_DEFAULT_CAPACITY;
  vec->_cemu_elem = cemu_elem;
  vec->_elems = malloc(vec->_capacity * cemu_impl(cemu_elem, size));

  return vec;
}

void delete_vector(vector_t *self)
{
  cemu_vector_delete(cemu_vector_data, self);
}

/**
 * ====================
 * api
 * ==================== 
 */

int vector_size(vector_t *self)
{
  return self->_size;
}

bool vector_empty(vector_t *self)
{
  return self->_size == 0;
}

void *vector_at(vector_t *self, int n)
{
  if (n >= self->_size) {
    return NULL;
  }

  return self->_elems + self->_elem_size * n;
}

void vector_insert(vector_t *self, int n, const void *value)
{
  if (n < 0 || n > self->_size) {
    return;
  }

  vector_expand(self);
  for (int i = self->_size; i > n; i--) {
    void *elem_next = self->_elems + self->_elem_size * i;
    void *elem_prev = self->_elems + self->_elem_size * (i - 1);

    memcpy(elem_next, elem_prev, self->_elem_size);
  }
  void *dest = self->_elems + self->_elem_size * n;
  void *src = cemu_impl(self->_cemu_elem, copy, value);
  memcpy(dest, src, self->_elem_size);
  free(src);

  self->_size += 1;
}

void vector_erase(vector_t *self, int n)
{
  if (n < 0 || n >= self->_size) {
    return;
  }

  cemu_impl(self->_cemu_elem, dtor, self->_elems + self->_elem_size * n);
  for (int i = n + 1; i < self->_size; i++) {
    void *elem_next = self->_elems + self->_elem_size * i;
    void *elem_prev = self->_elems + self->_elem_size * (i - 1);

    memcpy(elem_prev, elem_next, self->_elem_size);
  }

  self->_size -= 1;
  vector_shrink(self);
}

void vector_push_back(vector_t *self, const void *value)
{
  vector_insert(self, self->_size, value);
}

void vector_pop_back(vector_t *self)
{
  vector_erase(self, self->_size - 1);
}

void vector_clear(vector_t *self)
{
  while (!vector_empty(self)) {
    vector_pop_back(self);
  }
}

static void vector_expand(vector_t *self)
{
  if (self->_size < self->_capacity) return;
  if (self->_capacity * 2 < 0) return;

  self->_capacity *= 2;
  self->_elems = realloc(self->_elems, self->_capacity * self->_elem_size);
}

static void vector_shrink(vector_t *self)
{
  if (self->_capacity == VECTOR_DEFAULT_CAPACITY) return;
  if (self->_capacity <= self->_size * 2) return;

  self->_capacity /= 2;
  self->_elems = realloc(self->_elems, self->_capacity * self->_elem_size);
}