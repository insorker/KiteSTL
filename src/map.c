#include "map.h"
#include "malloc.h"

static int map_size(map_t *);
static void map_clear(map_t *);
static void map_insert(map_t *,  void *key, void *val);
static void map_erase(map_t *,  void *key);
static void *map_find(map_t *,  void *key);
static void map_extract(map_t *, vector_t *keys, vector_t *vals);

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

map_t *new_map(cemu_t cemu_key, cemu_t cemu_val)
{
  return cemu_map_new(&(treap_arg_t){cemu_key, cemu_val});
}

void delete_map(map_t *map)
{
  cemu_map_delete(map);
}

static int map_size(map_t *map)
{
  return map->_tr->size(map->_tr);
}

static void map_clear(map_t *map)
{
  map->_tr->clear(map->_tr);
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

static void map_extract(map_t *map, vector_t *keys, vector_t *vals)
{
  map->_tr->extract(map->_tr, &map->_tr->_root, keys, vals);
}