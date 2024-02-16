#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
#include <stdbool.h>
#include "cemu.h"

typedef struct
{
  size_t tsize;
  cemu_t cemu_elem;
} vector_type_t;

typedef struct vector_t
{
/** public */
  /* Return size */
  size_t (*size)(struct vector_t *);

  /* Test whether vector is empty */
  bool (*empty)(struct vector_t *);

  /* Access element */
  void *(*at)(struct vector_t *, size_t n);

  /* Insert elements */
  void (*insert)(struct vector_t *, size_t n, void *val);

  /* Erase elements */
  void (*erase)(struct vector_t *, size_t n);

  /* 	Add element at the end */
  void (*push_back)(struct vector_t *, void *val);

  /*	Delete last element */
  void (*pop_back)(struct vector_t *);

  /* 	Clear content */
  void (*clear)(struct vector_t *);

/** protected */
  /* Expand the capacity */
  void (*expand)(struct vector_t *);

  /* Shrink the capacity */
  void (*shrink)(struct vector_t *);

/** private */
  size_t _size;
  size_t _capacity;
  size_t _tsize;
  void *_elem;

  cemu_t _cemu_elem;

} vector_t;

cemu_t cemu_vector();

vector_t *new_vector(vector_type_t vector_type);
void delete_vector(vector_t *);

vector_type_t vector_int();

#endif