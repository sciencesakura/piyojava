#ifndef UTIL_LIST_H
#define UTIL_LIST_H

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct List List;
struct List {
  size_t capacity;
  intptr_t *front;
  intptr_t *rear;
};

void list_init(List *list, size_t init_cap);

void list_add(List *list, void *e);

void *list_find(const List *list, bool (*predicate)(const void *, va_list *), ...);

void list_clear(const List *list);

#endif
