#include "str.h"
#include <malloc.h>
#include <string.h>
#include <limits.h>

#define STRING_SCALING_FACTOR 2
#define STRING_MIN_CAPACITY 3
#define STRING_MAX_CAPACITY INT_MAX

static void string_expand(string_t *self);
static void string_shrink(string_t *self);

static const cemu_data_t cemu_string_data = { sizeof(string_t) };

static void cemu_string_dtor(cemu_data_t data, void *self)
{
  if (self == NULL) {
    return;
  }

  free(((string_t *)self)->_elems);
}

static void cemu_string_delete(cemu_data_t data, void *self)
{
  if (self == NULL) {
    return;
  }

  cemu_string_dtor(data, self);
  free(self);
}

static void *cemu_string_assign(cemu_data_t data, void *dest, const void *src)
{
  if (dest == NULL || src == NULL) {
    return dest;
  }

  string_t *str_dest = dest;
  const string_t *str_src = src;

  cemu_string_dtor(data, dest);
  memcpy(str_dest, str_src, sizeof(string_t));

  str_dest->_elems = malloc(str_dest->_capacity);
  memcpy(str_dest->_elems, str_src->_elems, str_src->_size);

  return dest;
}

static void *cemu_string_copy(cemu_data_t data, const void *src)
{
  if (src == NULL) {
    return NULL;
  }

  string_t *str_dest = malloc(sizeof(string_t));
  const string_t *str_src = src;

  memcpy(str_dest, str_src, sizeof(string_t));

  str_dest->_elems = malloc(str_dest->_capacity);
  memcpy(str_dest->_elems, str_src->_elems, str_src->_size);

  return str_dest;
}

static int cemu_string_cmp(cemu_data_t data, const void *lhs, const void *rhs)
{
  const string_t *str_lhs = lhs;
  const string_t *str_rhs = rhs;

  return strcmp(str_lhs->_elems, str_rhs->_elems);
}

cemu_t cemu_string()
{
  cemu_impl_t impl = {
    NULL,
    NULL,
    cemu_string_dtor,
    cemu_string_delete,
    cemu_string_assign,
    cemu_string_copy,
    cemu_string_cmp
  };
  return proto_cemu(cemu_string_data, impl, {});
}

string_t string()
{
  string_t str;

  str._size = 1;
  str._capacity = STRING_MIN_CAPACITY;
  str._elems = malloc(str._capacity);
  str._elems[0] = '\0';

  return str;
}

void _string(string_t self)
{
  cemu_string_dtor(cemu_string_data, &self);
}

string_t *new_string()
{
  string_t *str = malloc(sizeof(string_t));

  str->_size = 1;
  str->_capacity = STRING_MIN_CAPACITY;
  str->_elems = malloc(str->_capacity);
  str->_elems[0] = '\0';

  return str;
}

void delete_string(string_t *self)
{
  cemu_string_delete(cemu_string_data, self);
}

/**
 * ====================
 * api
 * ==================== 
 */

string_t *string_from(const char *s)
{
  string_t *self = new_string();

  if (s != NULL) {
    for (int i = 0; s[i]; i++) {
      string_push_back(self, s[i]);
    }
  }

  return self;
}

int string_size(string_t *self)
{
  return self->_size - 1;
}

bool string_empty(string_t *self)
{
  return self->_size == 0;
}

void string_resize(string_t *self, int n, const char val)
{
  if (n < 0) {
    return;
  }
  else if (n < string_size(self)) {
    string_shrink(self);
  }
  else if (n < INT_MAX) {
    while (n > string_size(self)) {
      string_push_back(self, val);
    }
  }
}

char *string_at(string_t *self, int n)
{
  if (n >= self->_size) {
    return NULL;
  }

  return self->_elems + n;
}

void string_insert(string_t *self, int n, const char c)
{
  if (n < 0 || n > string_size(self)) {
    return;
  }

  string_expand(self);
  for (int i = string_size(self) + 1; i > n; i--) {
    void *elem_next = self->_elems + i;
    void *elem_prev = self->_elems + (i - 1);

    memcpy(elem_next, elem_prev, 1);
  }
  void *dest = self->_elems + n;
  memcpy(dest, &c, 1);

  self->_size += 1;
}

void string_erase(string_t *self, int n)
{
  if (n < 0 || n >= string_size(self)) {
    return;
  }

  for (int i = n + 1; i < string_size(self) + 1; i++) {
    void *elem_next = self->_elems + i;
    void *elem_prev = self->_elems + (i - 1);

    memcpy(elem_prev, elem_next, 1);
  }

  self->_size -= 1;
  string_shrink(self);
}

void string_push_back(string_t *self, const char c)
{
  string_insert(self, string_size(self), c);
}

void string_pop_back(string_t *self)
{
  string_erase(self, string_size(self) - 1);
}

void string_clear(string_t *self)
{
  while (!string_empty(self)) {
    string_pop_back(self);
  }
}

static void string_expand(string_t *self)
{
  if (self->_size < self->_capacity) return;
  if (self->_capacity * STRING_SCALING_FACTOR < 0) return;

  self->_capacity *= STRING_SCALING_FACTOR;
  self->_elems = realloc(self->_elems, self->_capacity);
  self->_elems[self->_size - 1] = '\0';
}

static void string_shrink(string_t *self)
{
  if (self->_capacity == STRING_MIN_CAPACITY) return;
  if (self->_capacity <= self->_size * STRING_SCALING_FACTOR) return;

  self->_capacity /= STRING_SCALING_FACTOR;
  self->_elems = realloc(self->_elems, self->_capacity);
  self->_elems[self->_size - 1] = '\0';
}