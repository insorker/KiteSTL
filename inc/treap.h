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

typedef struct treap_node_t {
/** public **/
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
/** public **/
  /* Return container size */
  int (*size)(struct treap_t *);

  /* 	Clear content */
  void (*clear)(struct treap_t *);

  /* Insert Elements */
  void (*insert)(struct treap_t *, treap_node_t **p, void *key, void *val);

  /* Erase Elements */
  void (*erase)(struct treap_t *, treap_node_t **p, void *key);

  /* Get Element */
  void *(*find)(struct treap_t *, treap_node_t **p, void *key);

  /* Extract All */
  void (*extract)(struct treap_t *, treap_node_t **p, vector_t *keys, vector_t *vals);

/** protected **/
  /* push up */
  void (*pushup)(struct treap_t *, treap_node_t **p);

  /* rotate left */
  void (*zig)(struct treap_t *, treap_node_t **p);

  /* rotate right */
  void (*zag)(struct treap_t *, treap_node_t **p);

/** private **/
  cemu_t _cemu_key;
  cemu_t _cemu_val;
  treap_node_t *_root;

} treap_t;

int   cemu_treap_size();
void *cemu_treap_new(void *arg);
void  cemu_treap_delete(void *self);
cemu_t cemu_treap();

treap_node_t *new_treap_node(void *key, void *val);
void delete_treap_node(treap_t *, treap_node_t *);
treap_t *new_treap(cemu_t cemu_key, cemu_t cemu_val);
void delete_treap(treap_t *);

#endif