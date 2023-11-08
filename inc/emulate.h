#ifndef UTILS_H
#define UTILS_H

typedef int (*emulate_cmp_t)(void *lhs, void *rhs);
typedef void *(*emulate_clone_t)(void *val);
typedef void (*emulate_free_t)();

/* (int) */
int emulate_int_cmp(void *lhs, void *rhs);
void *emulate_int_clone(void *val);
void emulate_int_free(void *val);

/* (char *) */
int emulate_str_cmp(void *lhs, void *rhs);
void *emulate_str_clone(void *val);
void emulate_str_free(void *val);

#endif