#include "cemu.h"
#include <malloc.h>
#include <string.h>
#include <assert.h>

cemu_t cemu(
  cemu_new_t new,
  cemu_copy_t copy,
  cemu_dtor_t dtor,
  cemu_delete_t delete,
  cemu_op_assign_t assign,
  cemu_op_add_t add,
  cemu_op_sub_t sub,
  cemu_op_mul_t mul,
  cemu_op_div_t div,
  cemu_op_eq_t eq,
  cemu_op_ne_t ne,
  cemu_op_lt_t lt,
  cemu_op_gt_t gt,
  cemu_op_le_t le,
  cemu_op_ge_t ge)
{
  return (cemu_t){
    new, copy, dtor, delete, assign, add, sub, mul, div, eq, ne, lt, gt, le, ge
  };
}

void *cemu_int_new(void *arg)
{
  int *val = malloc(sizeof(int));
  assert(val != NULL);
  *val = *(int *)arg;
  return val;
}

void *cemu_int_copy(void *other)
{
  return cemu_int_new(other);
}

void cemu_int_dtor(void *self)
{
}

void  cemu_int_delete(void *self)
{
  free(self);
}

void cemu_int_op_assign(void *dest, void *src, int size)
{
  // void *copy = cemu_int_copy(src);
  // memcpy(dest, copy, sizeof(int));
  // free(copy);
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
    cemu_int_new, cemu_int_copy, cemu_int_dtor, cemu_int_delete, cemu_int_op_assign,
    cemu_int_op_add, cemu_int_op_sub, cemu_int_op_mul, cemu_int_op_div,
    cemu_int_op_eq, cemu_int_op_ne,
    cemu_int_op_lt, cemu_int_op_gt, cemu_int_op_le, cemu_int_op_ge
  );
}
// const cemu_t cemu_int = (cemu_t){
//   cemu_int_new, cemu_int_copy, cemu_int_dtor, cemu_int_op_assign,
//   cemu_int_op_add, cemu_int_op_sub, cemu_int_op_mul, cemu_int_op_div,
//   cemu_int_op_eq, cemu_int_op_ne,
//   cemu_int_op_lt, cemu_int_op_gt, cemu_int_op_le, cemu_int_op_ge
// };

// void *cemu_clone_int(void *val)
// {
//   int *val_copy = (int *)malloc(sizeof(int));
//   *val_copy = *(int *)val;
//   return val_copy;
// }

// void cemu_free_int(void *val)
// {
// }

// int cemu_cmp_int(void *lhs, void *rhs)
// {
//   int le = *(int *)lhs;
//   int ri = *(int *)rhs;

//   if (le == ri) return 0;
//   return le > ri ? 1 : -1;
// }


// void *cemu_clone_pchar(void *val)
// {
//   char **val_clone = (char **)malloc(sizeof(char *));

//   // deep copy start
//   int sz = strlen(*(char **)val) + 1;
//   char *content_clone = (char *)malloc(sz * sizeof(char));
//   strncpy(content_clone, *(char **)val, sz);
//   // deep copy end

//   *val_clone = content_clone;

//   return val_clone;
// }

// void cemu_free_pchar(void *val)
// {
//   free(*(char **)val);
// }

// int cemu_cmp_pchar(void *lhs, void *rhs)
// {
//   char *le = *(char **)lhs;
//   char *ri = *(char **)rhs;

//   return strcmp(le, ri);
// }