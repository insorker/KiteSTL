#ifndef TREAP_H
#define TREAP_H

#include "utils.h"

typedef struct treap_node_t {
/** public **/
  struct treap_node_t *_le, *_ri;
  void *_key, *_val;
  int _rand;
} treap_node_t;

typedef struct treap_utils_t {
  UtilsCmp cmp;
  UtilsClone clone;
  UtilsFree free;
} treap_utils_t;

typedef struct treap_t {
/** public **/
  void (*insert)(struct treap_t *, treap_node_t **p, void *key, void *val);
  void (*erase)(struct treap_t *, treap_node_t **p, void *key);
  void *(*find)(struct treap_t *, treap_node_t **p, void *key);

/** protected **/
  void (*zig)(struct treap_t *, treap_node_t **p);
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