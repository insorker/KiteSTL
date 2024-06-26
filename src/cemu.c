#include "cemu.h"
#include <malloc.h>
#include <string.h>

static int cemu_size(cemu_data_t data)
{
  return data._size;
}

static void *cemu_new(cemu_data_t data)
{
  return malloc(data._size);
}

static void cemu_dtor(cemu_data_t data, void *self)
{

}

static void cemu_delete(cemu_data_t data, void *self)
{
  if (self == NULL) {
    return;
  }

  free(self);
}

static void *cemu_assign(cemu_data_t data, void *dest, const void *src)
{
  if (dest == NULL || src == NULL) {
    return dest;
  }

  memcpy(dest, src, data._size);
  return dest;
}

static void *cemu_copy(cemu_data_t data, const void* src)
{
  if (src == NULL) {
    return NULL;
  }

  void *copy = malloc(data._size);
  memcpy(copy, src, data._size);
  return copy;
}

static int cemu_cmp(cemu_data_t data, const void *lhs, const void *rhs)
{
  if (lhs == NULL || rhs == NULL) {
    return 0;
  }

  return memcmp(lhs, rhs, data._size);
}

cemu_t _cemu(cemu_data_t data, cemu_impl_t impl, cemu_iter_t iter)
{
  return (cemu_t){
    .data = data,
    .impl = (cemu_impl_t){
      .size = impl.size ? impl.size : cemu_size,
      .new = impl.new ? impl.new : cemu_new,
      .dtor = impl.dtor ? impl.dtor : cemu_dtor,
      .delete = impl.delete ? impl.delete : cemu_delete,
      .assign = impl.assign ? impl.assign : cemu_assign,
      .copy = impl.copy ? impl.copy : cemu_copy,
      .cmp = impl.cmp ? impl.cmp : cemu_cmp
    },
    .iter = (cemu_iter_t){ }
  };
}