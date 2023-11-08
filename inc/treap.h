#ifndef TREAP_H
#define TREAP_H

#include "utils.h"
#include "vector.h"

typedef struct treap_node_t {
/** public **/
  struct treap_node_t *_le, *_ri;
  void *_key, *_val;
  int _rand;
  int _size;
} treap_node_t;

typedef struct treap_utils_t {
  UtilsCmp cmp;
  UtilsClone clone;
  UtilsFree free;
} treap_utils_t;

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
  UtilsCmp key_cmp;
  UtilsClone key_clone;
  UtilsClone val_clone;
  UtilsFree key_free;
  UtilsFree val_free;

  treap_node_t *_root;
} treap_t;

treap_node_t *new_treap_node(void *key, void *val);
void free_treap_node(treap_t *, treap_node_t *);
treap_t *new_treap(treap_utils_t key_utils, treap_utils_t val_utils);
void free_treap(treap_t *);

extern struct treap_utils_bundle {
  treap_utils_t tu_int;
  treap_utils_t tu_char;
} treap_utils;

#endif