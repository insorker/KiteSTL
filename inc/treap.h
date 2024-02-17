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

// typedef struct treap_cemu_t {
//   cemu_clone_t clone;
//   cemu_free_t free;
//   cemu_cmp_t cmp;
// } treap_cemu_t;
// typedef treap_cemu_t treap_cemu_key_t;
// typedef treap_cemu_t treap_cemu_val_t;

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
  // treap_cemu_key_t _cemu_key;
  // treap_cemu_val_t _cemu_val;
  treap_node_t *_root;
} treap_t;

treap_node_t *new_treap_node(void *key, void *val);
void free_treap_node(treap_t *, treap_node_t *);
treap_t *new_treap(cemu_t cemu_key, cemu_t cemu_val);
void free_treap(treap_t *);

// extern treap_cemu_t treap_cemu_int;
// extern treap_cemu_t treap_cemu_pchar;

#endif