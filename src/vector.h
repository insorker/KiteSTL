#ifndef VECTOR_H
#define VECTOR_H

typedef struct Vector {
/* public */
  int (*size)(struct Vector *);
  int (*empty)(struct Vector *);
  void *(*at)(struct Vector *, int n);
  void (*insert)(struct Vector *, int n, void *val);
  void (*remove)(struct Vector *, int n);
  void (*push_back)(struct Vector *, void *val);
  void (*pop_back)(struct Vector *);
  void (*clear)(struct Vector *);

/* protected */
  void (*expand)(struct Vector *);
  void (*shrink)(struct Vector *);

/* private */
  int _size;
  int _capacity;
  int _size_type;
  void *_elem;

} Vector;

Vector *new_vector(int size_type);
void free_vector(Vector *);

#endif
