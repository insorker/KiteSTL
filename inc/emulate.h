#ifndef EMULATE_H
#define EMULATE_H

/**
 * @brief Emulate copy-constructor in C++.
 * 
 * @param val Class instance's address.
 * @return void* Copy of class instance's address.
 * 
 * Always return a malloc pointer.
 * Remember to free return value with `free()` rather than `emulate_free()`.
 */
typedef void *(*emulate_clone_t)(void *val);

/**
 * @brief Emulate destructor in C++.
 * 
 * @param val Class instance's address.
 * 
 * NEVER assume that `free(val)` can be called, since it can be an address on stack OR an address not allocated by malloc.
 * If it's safe to call `free(val)`, call `free(val)` where it's necessary.
 */
typedef void (*emulate_free_t)(void *val);

/**
 * @brief Emulate operator== in C++.
 * 
 */
typedef int (*emulate_cmp_t)(void *lhs, void *rhs);

/* (int) */
void *emulate_clone_int(void *val);
void emulate_free_int(void *val);
int emulate_cmp_int(void *lhs, void *rhs);

/* (char *) */
void *emulate_clone_pchar(void *val);
void emulate_free_pchar(void *val);
int emulate_cmp_pchar(void *lhs, void *rhs);

#endif