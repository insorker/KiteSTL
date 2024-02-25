/**
 * @file vector.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-02-17
 * 
 * @copyright Copyright (c) 2024
 * 
 * cemu {
 *  size, dtor, assign
 * }
 */
#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
#include <stdbool.h>
#include "cemu.h"

/**
 * ====================
 * class
 * ==================== 
 */

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

/**
 * ====================
 * cemu
 * ==================== 
 */

int cemu_vector_size();
void *cemu_vector_new(void *arg);
void *cemu_vector_copy(void *other);
void cemu_vector_dtor(void *self);
void cemu_vector_delete(void *self);
void cemu_vector_op_assign(void *dest, void *src);
cemu_t cemu_vector();

/**
 * ====================
 * new / delete
 * ==================== 
 */

vector_t *new_vector(cemu_t cemu_int);
void delete_vector(vector_t *);

/**
 * ====================
 * api
 * ==================== 
 */

size_t vector_size(vector_t *);
bool vector_empty(vector_t *);
void *vector_at(vector_t *, size_t n);
void vector_insert(vector_t *, size_t n, void *val);
void vector_erase(vector_t *, size_t n);
void vector_push_back(vector_t *, void *val);
void vector_pop_back(vector_t *);
void vector_clear(vector_t *);

#endif