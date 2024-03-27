#include "cemu_int.h"
#include <malloc.h>
#include <string.h>

int cemu_int_size()
{
  return sizeof(int);
}

void *cemu_int_new(void *arg)
{
  int *val = malloc(sizeof(int));

  if (val == NULL) {
    return NULL;
  }
  else {
    *val = *(int *)arg;
  }

  return val;
}

void *cemu_int_copy(void *other)
{
  return cemu_int_new(other);
}

void cemu_int_dtor(void *self)
{
}

void cemu_int_delete(void *self)
{
  free(self);
}

void cemu_int_op_assign(void *dest, void *src)
{
  *(int *)dest = *(int *)src;
}

void *cemu_int_op_add(void *lhs, void *rhs)
{
  int val = *(int *)lhs + *(int *)rhs;
  return cemu_int_new(&(int){val});
}

void *cemu_int_op_sub(void *lhs, void *rhs)
{
  int val = *(int *)lhs - *(int *)rhs;
  return cemu_int_new(&(int){val});
}

void *cemu_int_op_mul(void *lhs, void *rhs)
{
  int val = *(int *)lhs * *(int *)rhs;
  return cemu_int_new(&(int){val});
}

void *cemu_int_op_div(void *lhs, void *rhs)
{
  int val = *(int *)lhs / *(int *)rhs;
  return cemu_int_new(&(int){val});
}

bool cemu_int_op_eq(void *lhs, void *rhs)
{
  return *(int *)lhs == *(int *)rhs;
}

bool cemu_int_op_ne(void *lhs, void *rhs)
{
  return *(int *)lhs != *(int *)rhs;
}

bool cemu_int_op_lt(void *lhs, void *rhs)
{
  return *(int *)lhs < *(int *)rhs;
}

bool cemu_int_op_gt(void *lhs, void *rhs)
{
  return *(int *)lhs > *(int *)rhs;
}

bool cemu_int_op_le(void *lhs, void *rhs)
{
  return *(int *)lhs <= *(int *)rhs;
}

bool cemu_int_op_ge(void *lhs, void *rhs)
{
  return *(int *)lhs >= *(int *)rhs;
}

cemu_t cemu_int()
{
  return cemu(
    cemu_int_size,
    cemu_int_new, cemu_int_copy, cemu_int_dtor, cemu_int_delete, cemu_int_op_assign,
    cemu_int_op_add, cemu_int_op_sub, cemu_int_op_mul, cemu_int_op_div,
    cemu_int_op_eq, cemu_int_op_ne,
    cemu_int_op_lt, cemu_int_op_gt, cemu_int_op_le, cemu_int_op_ge
  );
}