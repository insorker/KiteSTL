#ifndef CEMU_H
#define CEMU_H

#include <stdbool.h>

typedef struct {
  int _size;
} cemu_data_t;

typedef struct {
  int   (*size)  (cemu_data_t data);
  // Use `malloc` to creatd an instance and return a malloc pointer.
  void *(*new)   (cemu_data_t data);
  // Free member variables without free(self).
  void  (*dtor)  (cemu_data_t data, void *self);
  // Free member variables and free(self).
  // Param other should be address allocated by `malloc`.
  void  (*delete)(cemu_data_t data, void *self);
  void *(*assign)(cemu_data_t data, void *dest, const void *src);
  void *(*copy)  (cemu_data_t data, const void *src);
  int   (*cmp)   (cemu_data_t data, const void *lhs, const void *rhs);
} cemu_impl_t;

typedef struct {

} cemu_iter_t;

typedef struct {
  cemu_data_t data;
  cemu_impl_t impl;
  cemu_iter_t iter;
} cemu_t;

cemu_t _cemu(cemu_data_t data, cemu_impl_t impl, cemu_iter_t iter);
#define proto_cemu(data, impl, iter) \
  _cemu((cemu_data_t)data, (cemu_impl_t)impl, (cemu_iter_t)iter)

/**
 * @brief Create default cemu object with type T.
 * 
 */
#define cemu(T) \
  proto_cemu({sizeof(T)}, {}, {})

/**
 * @brief Syntactic sugar for calling cemu functins.
 * 
 */
#define cemu_impl(cemu, fn, ...) \
  (cemu.impl.fn(cemu.data,##__VA_ARGS__))

/**
 * @brief Copy object with cemu_copy.
 * There's only one parameter which is a pointer pointing to the object.
 * __VA_ARGS__ is for parameter with comma like this "&(pair){ 1, 2 }".
 * 
 */
#define proto_cemu_from(T, cemu, ...) \
  cemu_impl(cemu, copy, __VA_ARGS__)

/**
 * @brief Copy object with default cemu_copy which is shallow copy.
 * There's only one parameter which is a pointer pointing to the object.
 * __VA_ARGS__ is for parameter with comma like this "&(pair){ 1, 2 }".
 * 
 */
#define cemu_from(T, ...) \
  proto_cemu_from(T, cemu(T), __VA_ARGS__)

#endif