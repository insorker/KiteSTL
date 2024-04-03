#ifndef STR_H
#define STR_H

#include "cemu.h"

typedef struct {
  int _size;
  int _capacity;

  char *_elems;
} string_t;

cemu_t cemu_string();

string_t string();
void _string(string_t self);
string_t *new_string();
void delete_string(string_t *self);

string_t *string_from(const char *s);
string_t *string_replace(string_t *self, const char *s);
int   string_size(string_t *self);
bool  string_empty(string_t *self);
void  string_resize(string_t *self, int n, const char c);
char *string_at(string_t *self, int n);
void  string_insert(string_t *self, int n, const char c);
void  string_erase(string_t *self, int n);
void  string_push_back(string_t *self, const char c);
void  string_pop_back(string_t *self);
void  string_clear(string_t *self);

#endif