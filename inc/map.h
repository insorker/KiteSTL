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

/**
 * ====================
 * class
 * ==================== 
 */

typedef struct map_t {
  treap_t *_tr;
} map_t;

/**
 * ====================
 * cemu
 * ==================== 
 */

cemu_t cemu_map();

/**
 * ====================
 * new / delete
 * ==================== 
 */

map_t *new_map(cemu_t cemu_key, cemu_t cemu_val);
void delete_map(map_t *self);

/**
 * ====================
 * api
 * ==================== 
 */

int map_size(map_t *self);
void map_clear(map_t *self);
void map_insert(map_t *self,  void *key, void *val);
void map_erase(map_t *self,  void *key);
void *map_find(map_t *self,  void *key);
void map_extract(map_t *self, vector_t *keys, vector_t *vals);

#endif