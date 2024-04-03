#include "treap.h"
#include "cemu.h"
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

static void treap_pushup(treap_t *, treap_node_t **p);
static void treap_zig(treap_t *, treap_node_t **p);
static void treap_zag(treap_t *, treap_node_t **p);

/**
 * ====================
 * proto_cemu
 * ==================== 
 */

static const cemu_data_t cemu_treap_data = { sizeof(treap_t) };

static void cemu_treap_dtor(cemu_data_t data, void *self)
{
  delete_treap_node(self, ((treap_t *)self)->_root);
}

static void cemu_treap_delete(cemu_data_t data, void *self)
{
  cemu_treap_dtor(data, self);
  free(self);
}

cemu_t cemu_treap()
{
  cemu_impl_t impl = {
    NULL,
    NULL,
    cemu_treap_dtor,
    cemu_treap_delete
  };
  return proto_cemu(cemu_treap_data, impl, {});
}


/**
 * ====================
 * new / delete
 * ==================== 
 */

treap_node_t *new_treap_node(void *key, void *val)
{
  treap_node_t *node = (treap_node_t *)malloc(sizeof(treap_node_t));

  node->_le = node->_ri = NULL;
  node->_key = key;
  node->_val = val;
  node->_rand = rand();

  return node;
}

void delete_treap_node(treap_t *self, treap_node_t *self_node)
{
  if (!self_node) return;

  cemu_impl(self->_cemu_key, delete, self_node->_key);
  cemu_impl(self->_cemu_val, delete, self_node->_val);

  if (self_node->_le) delete_treap_node(self, self_node->_le);
  if (self_node->_ri) delete_treap_node(self, self_node->_ri);

  free(self_node);
}

treap_t *new_treap(cemu_t cemu_key, cemu_t cemu_val)
{
  treap_t *tr = malloc(sizeof(treap_t));

  tr->_cemu_key = cemu_key;
  tr->_cemu_val = cemu_val;
  tr->_root = NULL;

  return tr;
}

void delete_treap(treap_t *self)
{
  cemu_treap_delete(cemu_treap_data, self);
}

/**
 * ====================
 * api
 * ==================== 
 */

int treap_size(treap_t *self)
{
  if (!self->_root) return 0;
  return self->_root->_size;
}

void treap_clear(treap_t *self)
{
  delete_treap_node(self, self->_root);
  self->_root = NULL;
}

void treap_insert(treap_t *self, treap_node_t **p, void *key, void *val)
{
  if (!(*p)) {
    *p = new_treap_node(key, val);
  }
  else if (cemu_impl(self->_cemu_key, cmp, (*p)->_key, key) == 0) {
    cemu_impl(self->_cemu_val, delete, (*p)->_val);
    cemu_impl(self->_cemu_key, delete, key);
    (*p)->_val = val;
  }
  else if (cemu_impl(self->_cemu_key, cmp, (*p)->_key, key) > 0) {
    treap_insert(self, &(*p)->_le, key, val);
    if ((*p)->_val < (*p)->_le->_val) {
      treap_zig(self, p);
    }
  }
  else {
    treap_insert(self, &(*p)->_ri ,key, val);
    if ((*p)->_val < (*p)->_ri->_val) {
      treap_zag(self, p);
    }
  }

  treap_pushup(self, p);
}

void treap_erase(treap_t *self, treap_node_t **p, void *key)
{
  if (!(*p)) {
    cemu_impl(self->_cemu_key, delete, key);
    return;
  }
  else if (cemu_impl(self->_cemu_key, cmp, (*p)->_key, key) == 0) {
    if ((*p)->_le || (*p)->_ri) {
      if (!(*p)->_ri) {
        treap_zig(self, p);
        treap_erase(self, &(*p)->_ri, key);
      }
      else if (!(*p)->_le) {
        treap_zag(self, p);
        treap_erase(self, &(*p)->_le, key);
      }
      else if ((*p)->_le->_rand > (*p)->_ri->_rand) {
        treap_zig(self, p);
        treap_erase(self, &(*p)->_ri, key);
      }
      else {
        treap_zag(self, p);
        treap_erase(self, &(*p)->_le, key);
      }
    }
    else {
      delete_treap_node(self, *p);
      *p = NULL;
      cemu_impl(self->_cemu_key, delete, key);
    }
  }
  else if (cemu_impl(self->_cemu_key, cmp, (*p)->_key, key) > 0) {
    treap_erase(self, &(*p)->_le, key);
  }
  else {
    treap_erase(self, &(*p)->_ri, key);
  }

  treap_pushup(self, p);
}

void *treap_find(treap_t *self, treap_node_t **p, void *key)
{
  if (!(*p)) {
    cemu_impl(self->_cemu_key, delete, key);
    return NULL;
  }
  else if (cemu_impl(self->_cemu_key, cmp, (*p)->_key, key) == 0) {
    cemu_impl(self->_cemu_key, delete, key);
    return (*p)->_val;
  }
  else if (cemu_impl(self->_cemu_key, cmp, (*p)->_key, key) > 0) {
    return treap_find(self, &(*p)->_le, key);
  }
  else {
    return treap_find(self, &(*p)->_ri, key);
  }
}

void treap_extract(treap_t *self, treap_node_t **p, vector_t *keys, vector_t *vals)
{
  if (!(*p)) return;

  if ((*p)->_le) treap_extract(self, &(*p)->_le, keys, vals);
  vector_push_back(keys, cemu_impl(self->_cemu_key, copy, (*p)->_key));
  vector_push_back(vals, cemu_impl(self->_cemu_val, copy, (*p)->_val));
  if ((*p)->_ri) treap_extract(self, &(*p)->_ri, keys, vals);
}

static void treap_pushup(treap_t *self, treap_node_t **p)
{
  if (!(*p)) return;

  (*p)->_size = 1;
  if ((*p)->_le) (*p)->_size += (*p)->_le->_size;
  if ((*p)->_ri) (*p)->_size += (*p)->_ri->_size;
}

static void treap_zig(treap_t *self, treap_node_t **p)
{
  treap_node_t *q = (*p)->_le;
  (*p)->_le = q->_ri, q->_ri = (*p), (*p) = q;
  treap_pushup(self, &(*p)->_ri);
  treap_pushup(self, p);
}

static void treap_zag(treap_t *self, treap_node_t **p)
{
  treap_node_t *q = (*p)->_ri;
  (*p)->_ri = q->_le, q->_le = (*p), (*p) = q;
  treap_pushup(self, &(*p)->_le);
  treap_pushup(self, p);
}