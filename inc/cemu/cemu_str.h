#ifndef CEMU_STR_H
#define CEMU_STR_H

/**
 * @brief Emulate string class.
 * 
 */

#include <stdbool.h>
#include "cemu.h"

int   cemu_str_size();
void *cemu_str_new(void *arg);
void *cemu_str_copy(void *other);
void  cemu_str_dtor(void *self);
void  cemu_str_delete(void *self);
void  cemu_str_op_assign(void *dest, void *src);
void *cemu_str_op_add(void *lhs, void *rhs);
bool cemu_str_op_eq(void *lhs, void *rhs);
bool cemu_str_op_ne(void *lhs, void *rhs);
bool cemu_str_op_lt(void *lhs, void *rhs);
bool cemu_str_op_gt(void *lhs, void *rhs);
bool cemu_str_op_le(void *lhs, void *rhs);
bool cemu_str_op_ge(void *lhs, void *rhs);
cemu_t cemu_str();

#endif