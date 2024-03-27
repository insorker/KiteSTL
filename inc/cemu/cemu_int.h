#ifndef CEMU_INT_H
#define CEMU_INT_H

/**
 * @brief Emulate int class.
 * 
 */

#include <stdbool.h>
#include "cemu.h"

int   cemu_int_size();
void *cemu_int_new(void *arg);
void *cemu_int_copy(void *other);
void  cemu_int_dtor(void *self);
void  cemu_int_delete(void *self);
void  cemu_int_op_assign(void *dest, void *src);
void *cemu_int_op_add(void *lhs, void *rhs);
void *cemu_int_op_sub(void *lhs, void *rhs);
void *cemu_int_op_mul(void *lhs, void *rhs);
void *cemu_int_op_div(void *lhs, void *rhs);
bool cemu_int_op_eq(void *lhs, void *rhs);
bool cemu_int_op_ne(void *lhs, void *rhs);
bool cemu_int_op_lt(void *lhs, void *rhs);
bool cemu_int_op_gt(void *lhs, void *rhs);
bool cemu_int_op_le(void *lhs, void *rhs);
bool cemu_int_op_ge(void *lhs, void *rhs);
cemu_t cemu_int();

#endif