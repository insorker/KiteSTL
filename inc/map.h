/**
 * @file map.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-02-17
 * 
 * @copyright Copyright (c) 2024
 * 
 * cemu {
 *   same as treap
 * }
 */
#ifndef MAP_H
#define MAP_H

#include "treap.h"

typedef struct map_t {
/** public **/
  /* Return container size */
  int (*size)(struct map_t *);

  /* 	Clear content */
  void (*clear)(struct map_t *);

  /* Insert Elements */
  void (*insert)(struct map_t *,  void *key, void *val);

  /* Erase Elements */
  void (*erase)(struct map_t *, void *key);

  /* Get Element */
  void *(*find)(struct map_t *, void *key);

  /* Extract All */
  void (*extract)(struct map_t *, vector_t *keys, vector_t *vals);

/** private **/
  treap_t *_tr;
} map_t;

int   cemu_map_size();
void *cemu_map_new(void *arg);
void  cemu_map_delete(void *self);
cemu_t cemu_map();

map_t *new_map(cemu_t cemu_key, cemu_t cemu_val);
void delete_map(map_t *);

#endif