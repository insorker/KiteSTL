#include "emulate.h"
#include <malloc.h>
#include <string.h>

void *emulate_clone_int(void *val)
{
  int *val_copy = (int *)malloc(sizeof(int));
  *val_copy = *(int *)val;
  return val_copy;
}

void emulate_free_int(void *val)
{
}

int emulate_cmp_int(void *lhs, void *rhs)
{
  int le = *(int *)lhs;
  int ri = *(int *)rhs;

  if (le == ri) return 0;
  return le > ri ? 1 : -1;
}


void *emulate_clone_pchar(void *val)
{
  char **val_clone = (char **)malloc(sizeof(char *));

  // deep copy start
  int sz = strlen(*(char **)val) + 1;
  char *content_clone = (char *)malloc(sz * sizeof(char));
  strncpy(content_clone, *(char **)val, sz);
  // deep copy end

  *val_clone = content_clone;

  return val_clone;
}

void emulate_free_pchar(void *val)
{
  free(*(char **)val);
}

int emulate_cmp_pchar(void *lhs, void *rhs)
{
  char *le = *(char **)lhs;
  char *ri = *(char **)rhs;

  return strcmp(le, ri);
}