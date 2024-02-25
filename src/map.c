#include "map.h"
#include "malloc.h"

/**
 * ====================
 * cemu
 * ==================== 
 */

cemu_t cemu_map()
{
  return (cemu_t){
    cemu_map_size, cemu_map_new, NULL, NULL, cemu_map_delete
  };
}

int cemu_map_size()
{
  return sizeof(map_t);
}

void *cemu_map_new(void *arg)
{
  map_t *map = (map_t *)malloc(sizeof(map_t));

  map->size = map_size;
  map->clear = map_clear;
  map->insert = map_insert;
  map->erase = map_erase;
  map->find = map_find;
  map->extract = map_extract;

  map->_tr = cemu_treap_new(arg);

  return map;
}

void cemu_map_delete(void *self)
{
  map_t *map = self;
  delete_treap(map->_tr);
  free(map);
}

/**
 * ====================
 * new / delete
 * ==================== 
 */

map_t *new_map(cemu_t cemu_key, cemu_t cemu_val, treap_key_cmp_t key_cmp)
{
  return cemu_map_new(&(treap_arg_t){cemu_key, cemu_val, key_cmp});
}

void delete_map(map_t *map)
{
  cemu_map_delete(map);
}

/**
 * ====================
 * api
 * ==================== 
 */

int map_size(map_t *map)
{
  return map->_tr->size(map->_tr);
}

void map_clear(map_t *map)
{
  map->_tr->clear(map->_tr);
}

void map_insert(map_t *map, void *key, void *val)
{
  map->_tr->insert(map->_tr, &map->_tr->_root, key, val);
}

void map_erase(map_t *map, void *key)
{
  map->_tr->erase(map->_tr, &map->_tr->_root, key);
}

void *map_find(map_t *map, void *key)
{
  return map->_tr->find(map->_tr, &map->_tr->_root, key);
}

void map_extract(map_t *map, vector_t *keys, vector_t *vals)
{
  map->_tr->extract(map->_tr, &map->_tr->_root, keys, vals);
}