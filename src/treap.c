#include "treap.h"
#include "cemu.h"
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

static int treap_key_cmp(treap_t *self, void *lhs, void *rhs);
static void treap_pushup(treap_t *, treap_node_t **p);
static void treap_zig(treap_t *, treap_node_t **p);
static void treap_zag(treap_t *, treap_node_t **p);

/**
 * ====================
 * cemu
 * ==================== 
 */

cemu_t cemu_treap()
{
  return (cemu_t){
    cemu_treap_size, cemu_treap_new, NULL, cemu_treap_dtor, cemu_treap_delete
  };
}

int cemu_treap_size()
{
  return sizeof(treap_t);
}

void *cemu_treap_new(void *arg)
{
  treap_arg_t *tr_arg = arg;
  cemu_t cemu_key = tr_arg->cemu_key;
  cemu_t cemu_val = tr_arg->cemu_val;
  if (!cemu_key.copy || !cemu_key.delete) {
    return NULL;
  }
  else if (!cemu_val.copy || !cemu_val.delete) {
    return NULL;
  }
  else if (!tr_arg->key_cmp && (!cemu_key.eq || !cemu_key.gt)) {
    return NULL;
  }

  treap_t *tr = malloc(sizeof(treap_t));

  tr->size = treap_size;
  tr->clear = treap_clear;
  tr->insert = treap_insert;
  tr->erase = treap_erase;
  tr->find = treap_find;
  tr->extract = treap_extract;

  tr->key_cmp = treap_key_cmp;
  tr->pushup = treap_pushup;
  tr->zig = treap_zig;
  tr->zag = treap_zag;

  tr->_cemu_key = tr_arg->cemu_key;
  tr->_cemu_val = tr_arg->cemu_val;
  tr->_key_cmp = tr_arg->key_cmp;
  tr->_root = NULL;

  return tr;
}

void cemu_treap_dtor(void *self)
{
  treap_t *tr = self;
  delete_treap_node(tr, tr->_root);
}

void cemu_treap_delete(void *self)
{
  cemu_treap_dtor(self);
  free(self);
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

void delete_treap_node(treap_t *tr, treap_node_t *p)
{
  if (!p) return;

  tr->_cemu_key.delete(p->_key);
  tr->_cemu_val.delete(p->_val);

  if (p->_le) delete_treap_node(tr, p->_le);
  if (p->_ri) delete_treap_node(tr, p->_ri);

  free(p);
}

treap_t *new_treap(cemu_t cemu_key, cemu_t cemu_val, treap_key_cmp_t key_cmp)
{
  return cemu_treap_new(&(treap_arg_t){cemu_key, cemu_val, key_cmp});
}

void delete_treap(treap_t *tr)
{
  cemu_treap_delete(tr);
}

/**
 * ====================
 * api
 * ==================== 
 */

int treap_size(treap_t *tr)
{
  if (!tr->_root) return 0;
  return tr->_root->_size;
}

void treap_clear(treap_t *tr)
{
  delete_treap_node(tr, tr->_root);
  tr->_root = NULL;
}

void treap_insert(treap_t *tr, treap_node_t **p, void *key, void *val)
{
  if (!(*p)) {
    *p = new_treap_node(
      tr->_cemu_key.copy(key),
      tr->_cemu_val.copy(val));
  }
  else if (tr->key_cmp(tr, (*p)->_key, key) == 0) {
    tr->_cemu_val.delete((*p)->_val);
    (*p)->_val = tr->_cemu_val.copy(val);
  }
  else if (tr->key_cmp(tr, (*p)->_key, key) > 0) {
    tr->insert(tr, &(*p)->_le, key, val);
    if ((*p)->_val < (*p)->_le->_val) tr->zig(tr, p);
  }
  else {
    tr->insert(tr, &(*p)->_ri ,key, val);
    if ((*p)->_val < (*p)->_ri->_val) tr->zag(tr, p);
  }

  tr->pushup(tr, p);
}

void treap_erase(treap_t *tr, treap_node_t **p, void *key)
{
  if (!(*p)) return;
  else if (tr->key_cmp(tr, (*p)->_key, key) == 0) {
    if ((*p)->_le || (*p)->_ri) {
      if (!(*p)->_ri) {
        tr->zig(tr, p);
        tr->erase(tr, &(*p)->_ri, key);
      }
      else if (!(*p)->_le) {
        tr->zag(tr, p);
        tr->erase(tr, &(*p)->_le, key);
      }
      else if ((*p)->_le->_rand > (*p)->_ri->_rand) {
        tr->zig(tr, p);
        tr->erase(tr, &(*p)->_ri, key);
      }
      else {
        tr->zag(tr, p);
        tr->erase(tr, &(*p)->_le, key);
      }
    }
    else {
      delete_treap_node(tr, *p);
      *p = NULL;
    }
  }
  else if (tr->key_cmp(tr, (*p)->_key, key) > 0) {
    tr->erase(tr, &(*p)->_le, key);
  }
  else {
    tr->erase(tr, &(*p)->_ri, key);
  }

  tr->pushup(tr, p);
}

void *treap_find(treap_t *tr, treap_node_t **p, void *key)
{
  if (!(*p)) return NULL;
  else if (tr->key_cmp(tr, (*p)->_key, key) == 0) {
    return (*p)->_val;
  }
  else if (tr->key_cmp(tr, (*p)->_key, key) > 0) {
    return tr->find(tr, &(*p)->_le, key);
  }
  else {
    return tr->find(tr, &(*p)->_ri, key);
  }
}

void treap_extract(treap_t *tr, treap_node_t **p, vector_t *keys, vector_t *vals)
{
  if (!(*p)) return;

  if ((*p)->_le) treap_extract(tr, &(*p)->_le, keys, vals);
  keys->push_back(keys, (*p)->_key);
  vals->push_back(vals, (*p)->_val);
  if ((*p)->_ri) treap_extract(tr, &(*p)->_ri, keys, vals);
}

static int treap_key_cmp(treap_t *self, void *lhs, void *rhs)
{
  if (self->_key_cmp) {
    return self->_key_cmp(lhs, rhs);
  }
  else if (self->_cemu_key.eq && self->_cemu_key.gt) {
    if (self->_cemu_key.eq(lhs, rhs)) {
      return 0;
    }
    else if (self->_cemu_key.gt(lhs, rhs)) {
      return 1;
    }
    else {
      return -1;
    }
  }

  return 0;
}

static void treap_pushup(treap_t *tr, treap_node_t **p)
{
  if (!(*p)) return;

  (*p)->_size = 1;
  if ((*p)->_le) (*p)->_size += (*p)->_le->_size;
  if ((*p)->_ri) (*p)->_size += (*p)->_ri->_size;
}

static void treap_zig(treap_t *tr, treap_node_t **p)
{
  treap_node_t *q = (*p)->_le;
  (*p)->_le = q->_ri, q->_ri = (*p), (*p) = q;
  tr->pushup(tr, &(*p)->_ri);
  tr->pushup(tr, p);
}

static void treap_zag(treap_t *tr, treap_node_t **p)
{
  treap_node_t *q = (*p)->_ri;
  (*p)->_ri = q->_le, q->_le = (*p), (*p) = q;
  tr->pushup(tr, &(*p)->_le);
  tr->pushup(tr, p);
}