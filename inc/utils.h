#ifndef UTILS_H
#define UTILS_H

typedef int (*UtilsCmp)(void *lhs, void *rhs);
typedef void *(*UtilsClone)(void *val);
typedef void (*UtilsFree)();

#endif