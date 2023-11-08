#ifndef TREAP_H
#define TREAP_H

#include "emulate.h"
#include "vector.h"

typedef struct treap_node_t {
/** public **/
  struct treap_node_t *_le, *_ri;
  void *_key, *_val;
  int _rand;
  int _size;
} treap_node_t;

typedef struct treap_emulate_t {
  emulate_cmp_t cmp;
  emulate_clone_t clone;
  emulate_free_t free;
} treap_emulate_t;

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
  treap_emulate_t _emulate_key;
  treap_emulate_t _emulate_val;
  treap_node_t *_root;
} treap_t;

treap_node_t *new_treap_node(void *key, void *val);
void free_treap_node(treap_t *, treap_node_t *);
treap_t *new_treap(treap_emulate_t key, treap_emulate_t val);
void free_treap(treap_t *);

extern treap_emulate_t treap_emulate_int;
extern treap_emulate_t treap_emulate_str;

#endif