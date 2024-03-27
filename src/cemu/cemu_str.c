#include "cemu_str.h"
#include <malloc.h>
#include <string.h>

int cemu_str_size()
{
  return sizeof(char *);
}

void *cemu_str_new(void *arg)
{
  char **src = arg;
  char **dest = malloc(sizeof(char *));
  int sz = strlen(*src) + 1;

  if (dest == NULL) {
    return NULL;
  }
  else {
    *dest = malloc(sz * sizeof(char));
    if (*dest == NULL) {
      free(dest);
      return NULL;
    }
    strncpy(*dest, *src, sz);
  }

  return dest;
}

void *cemu_str_copy(void *other)
{
  return cemu_str_new(other);
}

void cemu_str_dtor(void *self)
{
  char **str = self;
  free(*str);
}

void cemu_str_delete(void *self)
{
  cemu_str_dtor(self);
  free(self);
}

void cemu_str_op_assign(void *dest, void *src)
{
  void *copy = cemu_str_copy(src);
  memcpy(dest, copy, cemu_str_size());
  free(copy);
}

void *cemu_str_op_add(void *lhs, void *rhs)
{
  char **lstr = lhs;
  char **rstr = rhs;
  char **res = malloc(sizeof(char *));
  int lsz = strlen(*lstr);
  int rsz = strlen(*rstr);

  if (res == NULL) {
    return res;
  }
  else {
    *res = malloc(lsz + rsz + 1);
    if (*res == NULL) {
      free(res);
      return NULL;
    }
    strncpy(*res, *lstr, lsz);
    strncpy(*res + rsz, *rstr, rsz + 1);
  }
  
  return res;
}

bool cemu_str_op_eq(void *lhs, void *rhs)
{
  char **lstr = lhs;
  char **rstr = rhs;
  int lsz = strlen(*lstr);
  int rsz = strlen(*rstr);

  if (lsz != rsz) {
    return false;
  }
  else {
    for (int i = 0; i < lsz; i++) {
      if ((*lstr)[i] != (*rstr)[i]) {
        return false;
      }
    }
  }

  return true;
}

bool cemu_str_op_ne(void *lhs, void *rhs)
{
  return 1 - cemu_str_op_eq(lhs, rhs);
}

bool cemu_str_op_lt(void *lhs, void *rhs)
{
  char **lstr = lhs;
  char **rstr = rhs;
  int lsz = strlen(*lstr);
  int rsz = strlen(*rstr);
  int sz = lsz > rsz ? rsz : lsz;

  for (int i = 0; i < sz; i++) {
    if ((*lstr)[i] == (*rstr)[i]) {
      continue;
    }
    else if ((*lstr)[i] < (*rstr)[i]) {
      return true;
    }
    else {
      return false;
    }
  }

  if (lsz < rsz) {
    return true;
  }
  return false;
}

bool cemu_str_op_gt(void *lhs, void *rhs)
{
  return 1 - cemu_str_op_lt(lhs, rhs);
}

bool cemu_str_op_le(void *lhs, void *rhs)
{
  return cemu_str_op_eq(lhs, rhs) || cemu_str_op_lt(lhs, rhs);
}

bool cemu_str_op_ge(void *lhs, void *rhs)
{
  return cemu_str_op_eq(lhs, rhs) || cemu_str_op_gt(lhs, rhs);
}

cemu_t cemu_str()
{
  return cemu(
    cemu_str_size,
    cemu_str_new, cemu_str_copy, cemu_str_dtor, cemu_str_delete, cemu_str_op_assign,
    cemu_str_op_add, NULL, NULL, NULL,
    cemu_str_op_eq, cemu_str_op_ne,
    cemu_str_op_lt, cemu_str_op_gt, cemu_str_op_le, cemu_str_op_ge
  );
}