#ifndef VECTOR_H
#define VECTOR_H

typedef struct vector_t {
/** public */
  /* Return size */
  int (*size)(struct vector_t *);

  /* Test whether vector is empty */
  int (*empty)(struct vector_t *);

  /* Access element */
  void *(*at)(struct vector_t *, int n);

  /* Insert elements */
  void (*insert)(struct vector_t *, int n, void *val);

  /* Remove elements */
  void (*remove)(struct vector_t *, int n);

  /* 	Add element at the end */
  void (*push_back)(struct vector_t *, void *val);

  /*	Delete last element */
  void (*pop_back)(struct vector_t *);

  /* 	Clear content */
  void (*clear)(struct vector_t *);

/** protected */
  /* Expand the capacity */
  void (*expand)(struct vector_t *);

  /* Shrink the capacity */
  void (*shrink)(struct vector_t *);

/** private */
  int _size;
  int _capacity;
  int _size_type;
  void *_elem;

} vector_t;

vector_t *new_vector(int size_type);
void free_vector(vector_t *);

#endif
