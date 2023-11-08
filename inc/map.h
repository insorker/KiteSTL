#ifndef MAP_H
#define MAP_H

#include "treap.h"

typedef struct map_t {
/** public **/
  /* Return container size */
  int (*size)(struct map_t *);

  /* Insert Elements */
  void (*insert)(struct map_t *,  void *key, void *val);

  /* Erase Elements */
  void (*erase)(struct map_t *, void *key);

  /* Get Element */
  void *(*find)(struct map_t *, void *key);

/** private **/
  treap_t *_tr;
} map_t;

map_t *new_map(treap_utils_t key_utils, treap_utils_t val_utils);
void free_map(map_t *);

#endif