/**
 * @file treap.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-02-17
 * 
 * @copyright Copyright (c) 2024
 * 
 * cemu {
 *   copy, delete, eq, gt
 * }
 */
#ifndef TREAP_H
#define TREAP_H

#include "cemu.h"
#include "vector.h"
#include <stddef.h>

/**
 * ====================
 * class
 * ==================== 
 */

typedef struct treap_node_t {
  struct treap_node_t *_le, *_ri;
  void *_key, *_val;
  int _rand;
  int _size;
} treap_node_t;

typedef struct {
  cemu_t cemu_key;
  cemu_t cemu_val;
} treap_arg_t;

typedef struct treap_t {
  cemu_t _cemu_key;
  cemu_t _cemu_val;
  treap_node_t *_root;
} treap_t;

/**
 * ====================
 * cemu
 * ==================== 
 */

cemu_t cemu_treap();

/**
 * ====================
 * new / delete
 * ==================== 
 */

treap_node_t *new_treap_node(void *key, void *val);
void delete_treap_node(treap_t *self, treap_node_t *self_node);
treap_t *new_treap(cemu_t cemu_key, cemu_t cemu_val);
void delete_treap(treap_t *self);

/**
 * ====================
 * api
 * ==================== 
 */

int treap_size(treap_t *self);
void treap_clear(treap_t *self);
void treap_insert(treap_t *self, treap_node_t **p, void *key, void *val);
void treap_erase(treap_t *self, treap_node_t **p, void *key);
void *treap_find(treap_t *self, treap_node_t **p, void *key);
void treap_extract(treap_t *self, treap_node_t **p, vector_t *keys, vector_t *vals);

#endif