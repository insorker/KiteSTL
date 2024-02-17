#include "treap.h"
#include "cemu.h"
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static int treap_size(treap_t *);
static void treap_clear(treap_t *);
static void treap_insert(treap_t *, treap_node_t **p, void *key, void *val);
static void treap_erase(treap_t *, treap_node_t **p, void *key);
static void *treap_find(treap_t *, treap_node_t **p, void *key);
static void treap_extract(treap_t *, treap_node_t **p, vector_t *keys, vector_t *vals);

static void treap_pushup(treap_t *, treap_node_t **p);
static void treap_zig(treap_t *, treap_node_t **p);
static void treap_zag(treap_t *, treap_node_t **p);

cemu_t cemu_treap()
{
  return (cemu_t){
    cemu_treap_size, cemu_treap_new, NULL, NULL, cemu_treap_delete
  };
}

int cemu_treap_size()
{
  return sizeof(treap_t);
}

void *cemu_treap_new(void *arg)
{
  treap_arg_t *treap_arg = arg;
  treap_t *tr = malloc(sizeof(treap_t));

  tr->size = treap_size;
  tr->clear = treap_clear;
  tr->insert = treap_insert;
  tr->erase = treap_erase;
  tr->find = treap_find;
  tr->extract = treap_extract;

  tr->pushup = treap_pushup;
  tr->zig = treap_zig;
  tr->zag = treap_zag;

  tr->_cemu_key = treap_arg->cemu_key;
  tr->_cemu_val = treap_arg->cemu_val;
  tr->_root = NULL;

  return tr;
}

void cemu_treap_delete(void *self)
{
  treap_t *tr = self;
  delete_treap_node(tr, tr->_root);
  free(tr);
}

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

treap_t *new_treap(cemu_t cemu_key, cemu_t cemu_val)
{
  return cemu_treap_new(&(treap_arg_t){ cemu_key, cemu_val });
}

void delete_treap(treap_t *tr)
{
  cemu_treap_delete(tr);
}

static int treap_size(treap_t *tr)
{
  if (!tr->_root) return 0;
  return tr->_root->_size;
}

static void treap_clear(treap_t *tr)
{
  delete_treap_node(tr, tr->_root);
  tr->_root = NULL;
}

static void treap_insert(treap_t *tr, treap_node_t **p, void *key, void *val)
{
  if (!(*p)) {
    *p = new_treap_node(
      tr->_cemu_key.copy(key),
      tr->_cemu_val.copy(val));
  }
  else if (tr->_cemu_key.eq((*p)->_key, key)) {
    tr->_cemu_val.delete((*p)->_val);
    (*p)->_val = tr->_cemu_val.copy(val);
  }
  else if (tr->_cemu_key.gt((*p)->_key, key) > 0) {
    tr->insert(tr, &(*p)->_le, key, val);
    if ((*p)->_val < (*p)->_le->_val) tr->zig(tr, p);
  }
  else {
    tr->insert(tr, &(*p)->_ri ,key, val);
    if ((*p)->_val < (*p)->_ri->_val) tr->zag(tr, p);
  }

  tr->pushup(tr, p);
}

static void treap_erase(treap_t *tr, treap_node_t **p, void *key)
{
  if (!(*p)) return;
  else if (tr->_cemu_key.eq((*p)->_key, key)) {
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
  else if (tr->_cemu_key.gt((*p)->_key, key)) {
    tr->erase(tr, &(*p)->_le, key);
  }
  else {
    tr->erase(tr, &(*p)->_ri, key);
  }

  tr->pushup(tr, p);
}

static void *treap_find(treap_t *tr, treap_node_t **p, void *key)
{
  if (!(*p)) return NULL;
  else if (tr->_cemu_key.eq((*p)->_key, key)) {
    return (*p)->_val;
  }
  else if (tr->_cemu_key.gt((*p)->_key, key)) {
    return tr->find(tr, &(*p)->_le, key);
  }
  else {
    return tr->find(tr, &(*p)->_ri, key);
  }
}

static void treap_extract(treap_t *tr, treap_node_t **p, vector_t *keys, vector_t *vals)
{
  if (!(*p)) return;

  if ((*p)->_le) treap_extract(tr, &(*p)->_le, keys, vals);
  keys->push_back(keys, (*p)->_key);
  vals->push_back(vals, (*p)->_val);
  if ((*p)->_ri) treap_extract(tr, &(*p)->_ri, keys, vals);
}

static void treap_pushup(treap_t *tr, treap_node_t **p)
{
  if (!(*p)) return;

  (*p)->_size = 1;
  if ((*p)->_le) (*p)->_size += (*p)->_le->_size;
  if ((*p)->_ri) (*p)->_size += (*p)->_ri->_size;

  assert((*p)->_size >= 0);
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

// treap_cemu_t treap_cemu_int = {
//   cemu_clone_int, cemu_free_int, cemu_cmp_int
// };
// treap_cemu_t treap_cemu_pchar = {
//   cemu_clone_pchar, cemu_free_pchar, cemu_cmp_pchar
// };