#ifndef CEMU_H
#define CEMU_H

#include <stdbool.h>

/**
 * @brief Emulate class size.
 * 
 * e.g. int: sizeof(int)
 *      struct: sizeof(struct)
 */
typedef int (*cemu_size_t)();

/**
 * @brief Emulate new.
 * 
 * 1. Use `malloc` to creatd an instance and return a malloc pointer.
 * 2. 准确来说这里模拟的是使用 new 构造。另一种可行的方法是返回一个固定大小，比如 void* 可以看作 8 比特大小的内存，
 *    这样就可以不使用 malloc 而获得一个栈上的实例，但这种方法显然没办法处理过大的变量，所以放弃。
 */
typedef void *(*cemu_new_t)(void *arg);

/**
 * @brief Emulate copy constructor.
 * 
 * 1. Use `malloc` to create an instance and return a malloc pointer.
 * 2. 同上。
 */
typedef void *(*cemu_copy_t)(void *other);

/**
 * @brief Emulate destructor.
 * 
 * 1. 显然，如果实例在栈上，那么只需要 free 实例的各个组件即可。如果是 new / malloc 出来的，那么就需要 free malloc
 *    的空间。所以这里区分了 dtor 和 delete，因为栈上的构造做不出来，所以只有一个 new。
 */
typedef void (*cemu_dtor_t)(void *self);

/**
 * @brief Emulate delete.
 * 
 * 1. Param other should be address allocated by `malloc`.
 * 2. 同上。
 */
typedef void (*cemu_delete_t)(void *self);

/**
 * @brief Emulate operator=.
 * 
 */
typedef void (*cemu_op_assign_t)(void *dest, void *src);

/**
 * @brief Emulate operator+.
 * 
 */
typedef void *(*cemu_op_add_t)(void *lhs, void *rhs);

/**
 * @brief Emulate operator-.
 * 
 */
typedef void *(*cemu_op_sub_t)(void *lhs, void *rhs);

/**
 * @brief Emulate operator*.
 * 
 */
typedef void *(*cemu_op_mul_t)(void *lhs, void *rhs);

/**
 * @brief Emulate operator/.
 * 
 */
typedef void *(*cemu_op_div_t)(void *lhs, void *rhs);

/**
 * @brief Emulate operator==.
 * 
 */
typedef bool (*cemu_op_eq_t)(void *lhs, void *rhs);

/**
 * @brief Emulate operator!=.
 * 
 */
typedef bool (*cemu_op_ne_t)(void *lhs, void *rhs);

/**
 * @brief Emulate operator<.
 * 
 */
typedef bool (*cemu_op_lt_t)(void *lhs, void *rhs);

/**
 * @brief Emulate operator>.
 * 
 */
typedef bool (*cemu_op_gt_t)(void *lhs, void *rhs);

/**
 * @brief Emulate operator<=.
 * 
 */
typedef bool (*cemu_op_le_t)(void *lhs, void *rhs);

/**
 * @brief Emulate operator>=.
 * 
 */
typedef bool (*cemu_op_ge_t)(void *lhs, void *rhs);

/**
 * @brief Emulate class.
 * 
 */
typedef struct {
  cemu_size_t size;
  cemu_new_t new;
  cemu_copy_t copy;
  cemu_dtor_t dtor;
  cemu_delete_t delete;
  cemu_op_assign_t assign;
  cemu_op_add_t add;
  cemu_op_sub_t sub;
  cemu_op_mul_t mul;
  cemu_op_div_t div;
  cemu_op_eq_t eq;
  cemu_op_ne_t ne;
  cemu_op_lt_t lt;
  cemu_op_gt_t gt;
  cemu_op_le_t le;
  cemu_op_ge_t ge;
} cemu_t;

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
  cemu_op_ge_t ge
);

/**
 * @brief Emulate int class.
 * 
 */
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