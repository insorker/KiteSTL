#include "cemu.h"

cemu_t cemu(
  cemu_size_t size,
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
    size, new, copy, dtor, delete, assign, add, sub, mul, div, eq, ne, lt, gt, le, ge
  };
}