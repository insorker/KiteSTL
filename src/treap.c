#include "treap.h"
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

treap_node_t *new_treap_node(void *key, void *val)
{
  treap_node_t *node = (treap_node_t *)malloc(sizeof(treap_node_t));

  node->_le = node->_ri = NULL;
  node->_key = key;
  node->_val = val;
  node->_rand = rand();

  return node;
}

void free_treap_node(treap_t *tr, treap_node_t *p)
{
  if (!p) return;

  tr->key_free(p->_key);
  tr->val_free(p->_val);

  if (p->_le) {
    free_treap_node(tr, p->_le);
  }
  if (p->_ri) {
    free_treap_node(tr, p->_ri);
  }
  free(p);
}

treap_t *new_treap(treap_utils_t key_utils, treap_utils_t val_utils)
{
  treap_t *tr = (treap_t *)malloc(sizeof(treap_t));

  tr->size = treap_size;
  tr->clear = treap_clear;
  tr->insert = treap_insert;
  tr->erase = treap_erase;
  tr->find = treap_find;
  tr->extract = treap_extract;

  tr->key_cmp = key_utils.cmp;
  tr->key_clone = key_utils.clone;
  tr->val_clone = val_utils.clone;
  tr->key_free= key_utils.free;
  tr->val_free = val_utils.free;

  tr->pushup = treap_pushup;
  tr->zig = treap_zig;
  tr->zag = treap_zag;

  tr->_root = NULL;

  return tr;
}

void free_treap(treap_t *tr)
{
  free_treap_node(tr, tr->_root);
  free(tr);
}

static int treap_size(treap_t *tr)
{
  return tr->_root->_size;
}

static void treap_clear(treap_t *tr)
{
  free_treap_node(tr, tr->_root);
  tr->_root = NULL;
}

static void treap_insert(treap_t *tr, treap_node_t **p, void *key, void *val)
{
  if (!(*p)) {
    (*p) = new_treap_node(
      tr->key_clone(key),
      tr->val_clone(val));
  }
  else if (tr->key_cmp((*p)->_key, key) == 0) {
    tr->val_free((*p)->_val);
    (*p)->_val = tr->val_clone(val);
  }
  else if (tr->key_cmp((*p)->_key, key) > 0) {
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
  else if (tr->key_cmp((*p)->_key, key) == 0) {
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
      free_treap_node(tr, *p);
      *p = NULL;
    }
  }
  else if (tr->key_cmp((*p)->_key, key) > 0) {
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
  else if (tr->key_cmp((*p)->_key, key) == 0) {
    return (*p)->_val;
  }
  else if (tr->key_cmp((*p)->_key, key) > 0) {
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
  tr->pushup(tr, p);
}

static void treap_zag(treap_t *tr, treap_node_t **p)
{
  treap_node_t *q = (*p)->_ri;
  (*p)->_ri = q->_le, q->_le = (*p), (*p) = q;
  tr->pushup(tr, p);
}

static int treap_utils_int_cmp(void *lhs, void *rhs)
{
  int le = *(int *)lhs;
  int ri = *(int *)rhs;

  if (le == ri) return 0;
  return le > ri ? 1 : -1;
}

static void *treap_utils_int_clone(void *val)
{
  int *val_copy = (int *)malloc(sizeof(int));
  *val_copy = *(int *)val;
  return val_copy;
}

static void treap_utils_int_free(void *val)
{
  free((int *)val);
}

static int treap_utils_char_cmp(void *lhs, void *rhs)
{
  char *le = (char *)lhs;
  char *ri = (char *)rhs;

  return strcmp(le, ri);
}

static void *treap_utils_char_clone(void *val)
{
  int sz = 0;
  while (((char *)val)[sz++]);

  char *val_copy = (char *)malloc(sz * sizeof(char));
  strcpy(val_copy, val);
  val_copy[sz - 1] = '\0';

  return val_copy;
}

static void treap_utils_char_free(void *val)
{
  free((char *)val);
}

struct treap_utils_bundle treap_utils = {
  { treap_utils_int_cmp, treap_utils_int_clone, treap_utils_int_free },
  { treap_utils_char_cmp, treap_utils_char_clone, treap_utils_char_free }
};