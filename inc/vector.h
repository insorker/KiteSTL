#ifndef VECTOR_H
#define VECTOR_H

typedef struct Vector {
/** public */
  /* Return size */
  int (*size)(struct Vector *);

  /* Test whether vector is empty */
  int (*empty)(struct Vector *);

  /* Access element */
  void *(*at)(struct Vector *, int n);

  /* Insert elements */
  void (*insert)(struct Vector *, int n, void *val);

  /* Remove elements */
  void (*remove)(struct Vector *, int n);

  /* 	Add element at the end */
  void (*push_back)(struct Vector *, void *val);

  /*	Delete last element */
  void (*pop_back)(struct Vector *);

  /* 	Clear content */
  void (*clear)(struct Vector *);


/** protected */
  /* Expand the capacity */
  void (*expand)(struct Vector *);

  /* Shrink the capacity */
  void (*shrink)(struct Vector *);

/** private */
  int _size;
  int _capacity;
  int _size_type;
  void *_elem;

} vector_t;

vector_t *new_vector(int size_type);
void free_vector(vector_t *);

#endif
