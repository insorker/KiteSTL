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

typedef struct {
  int _size;
  int _capacity;

  void *_elems;
  int _elem_size;
  cemu_t _cemu_elem;
} vector_t;

/**
 * ====================
 * cemu
 * ==================== 
 */

cemu_t cemu_vector();

/**
 * ====================
 * construct
 * ==================== 
 */

vector_t vector(cemu_t cemu_elem);
void _vector(vector_t self);
vector_t *new_vector(cemu_t cemu_elem);
void delete_vector(vector_t *self);

/**
 * ====================
 * api
 * ==================== 
 */

int   vector_size(vector_t *self);
bool  vector_empty(vector_t *self);
void  vector_resize(vector_t *self, int n, void *val);
void *vector_at(vector_t *self, int n);
void  vector_insert(vector_t *self, int n, void *val);
void  vector_erase(vector_t *self, int n);
void  vector_push_back(vector_t *self, void *val);
void  vector_pop_back(vector_t *self);
void  vector_clear(vector_t *self);

#endif