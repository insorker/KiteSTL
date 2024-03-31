#ifndef CEMU_H
#define CEMU_H

#include <stdbool.h>

typedef struct {
  int _size;
} cemu_data_t;

typedef struct {
  int   (*size)  (cemu_data_t data);
  void *(*new)   (cemu_data_t data);
  void  (*dtor)  (cemu_data_t data, void *self);
  void  (*delete)(cemu_data_t data, void *self);
  void  (*assign)(cemu_data_t data, void *dest, void *src);
  void *(*copy)  (cemu_data_t data, void *src);
  int   (*cmp)   (cemu_data_t data, void *lhs, void *rhs);
} cemu_impl_t;

typedef struct {
  cemu_data_t data;
  cemu_impl_t impl;
} cemu_t;

cemu_t cemu_init(cemu_data_t data, cemu_impl_t impl);
#define cemu(T, impl) \
  cemu_init((cemu_data_t){ sizeof(T) }, (cemu_impl_t)impl)
#define cemu_impl(cemu, fn, ...) \
  (cemu.impl.fn(cemu.data,##__VA_ARGS__))
#define cemu_make(type, value) &(type){value}
#define cemu_get(type, value) *(type*)(value)

  /**
   * 1. Use `malloc` to creatd an instance and return a malloc pointer.
   * 2. 准确来说这里模拟的是使用 new 构造。另一种可行的方法是返回一个固定大小，比如 void* 可以看作 8 比特大小的内存，
   *    这样就可以不使用 malloc 而获得一个栈上的实例，但这种方法显然没办法处理过大的变量，所以放弃。
   * 
   */

  /**
   * 1. Param other should be address allocated by `malloc`.
   * 2. 显然，如果实例在栈上，那么只需要 free 实例的各个组件即可。如果是 new / malloc 出来的，那么就需要 free malloc
   *    的空间。所以这里区分了 dtor 和 delete，因为栈上的构造做不出来，所以只有一个 new。
   * 
   */
#endif