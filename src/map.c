#include "map.h"
#include "malloc.h"

/**
 * ====================
 * cemu
 * ==================== 
 */

static cemu_data_t cemu_map_data = { sizeof(cemu_data_t) };

static void cemu_map_dtor(cemu_data_t data, void *self)
{
  delete_treap(((map_t *)self)->_tr);
}

static void cemu_map_delete(cemu_data_t data, void *self)
{
  cemu_map_dtor(data, self);
  free(self);
}

/**
 * ====================
 * new / delete
 * ==================== 
 */

map_t *new_map(cemu_t cemu_key, cemu_t cemu_val)
{
  map_t *map = (map_t *)malloc(sizeof(map_t));
  
  map->_tr = new_treap(cemu_key, cemu_val);

  return map;
}

void delete_map(map_t *map)
{
  cemu_map_delete(cemu_map_data, map);
}

/**
 * ====================
 * api
 * ==================== 
 */

int map_size(map_t *self)
{
  return treap_size(self->_tr);
}

void map_clear(map_t *self)
{
  treap_clear(self->_tr);
}

void map_insert(map_t *self, void *key, void *val)
{
  treap_insert(self->_tr, &self->_tr->_root, key, val);
}

void map_erase(map_t *self, void *key)
{
  treap_erase(self->_tr, &self->_tr->_root, key);
}

void *map_find(map_t *self, void *key)
{
  return treap_find(self->_tr, &self->_tr->_root, key);
}

void map_extract(map_t *self, vector_t *keys, vector_t *vals)
{
  treap_extract(self->_tr, &self->_tr->_root, keys, vals);
}