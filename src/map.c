#include "map.h"
#include "malloc.h"

static void map_insert(map_t *,  void *key, void *val);
static void map_erase(map_t *,  void *key);
static void *map_find(map_t *,  void *key);

map_t *new_map(treap_utils_t key_utils, treap_utils_t val_utils)
{
  map_t *map = (map_t *)malloc(sizeof(map_t));

  map->insert = map_insert;
  map->erase = map_erase;
  map->find = map_find;

  map->_tr = new_treap(key_utils, val_utils);

  return map;
}

void free_map(map_t *map)
{
  free_treap(map->_tr);
  free(map);
}

static void map_insert(map_t *map,  void *key, void *val)
{
  map->_tr->insert(map->_tr, &map->_tr->_root, key, val);
}

static void map_erase(map_t *map,  void *key)
{
  map->_tr->erase(map->_tr, &map->_tr->_root, key);
}

static void *map_find(map_t *map,  void *key)
{
  return map->_tr->find(map->_tr, &map->_tr->_root, key);
}