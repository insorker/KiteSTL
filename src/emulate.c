#include "emulate.h"
#include <malloc.h>
#include <string.h>

int emulate_int_cmp(void *lhs, void *rhs)
{
  int le = *(int *)lhs;
  int ri = *(int *)rhs;

  if (le == ri) return 0;
  return le > ri ? 1 : -1;
}

void *emulate_int_clone(void *val)
{
  int *val_copy = (int *)malloc(sizeof(int));
  *val_copy = *(int *)val;
  return val_copy;
}

void emulate_int_free(void *val)
{
  free((int *)val);
}

int emulate_str_cmp(void *lhs, void *rhs)
{
  char *le = (char *)lhs;
  char *ri = (char *)rhs;

  return strcmp(le, ri);
}

void *emulate_str_clone(void *val)
{
  int sz = 0; while (((char *)val)[sz++] != '\0');
  char *val_clone = (char *)malloc(sz * sizeof(char));

  strncpy(val_clone, val, sz);

  return val_clone;
}

void emulate_str_free(void *val)
{
  free((char *)val);
}