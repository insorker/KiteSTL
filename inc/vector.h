#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
#include <stdbool.h>
#include "emulate.h"

typedef struct vector_emulate_t
{
  size_t tsize;
  emulate_clone_t clone;
  emulate_free_t free;
} vector_emulate_t;

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

  vector_emulate_t _emulate_elem;

} vector_t;

vector_t *new_vector(vector_emulate_t emulate_elem);
void free_vector(vector_t *);

extern vector_emulate_t vector_emulate_int;
extern vector_emulate_t vector_emulate_pchar;

#endif