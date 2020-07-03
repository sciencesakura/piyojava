#include "util/list.h"

#include <stdlib.h>

void list_init(List *list, size_t init_cap)
{
  list->capacity = init_cap == 0 ? 16 : init_cap;
  list->front = calloc(list->capacity, sizeof(intptr_t));
  list->rear = list->front;
}

void list_add(List *list, void *e)
{
  if (list->rear - list->front == list->capacity) {
    size_t new_cap = list->capacity * 2;
    list->front = realloc(list->front, new_cap * sizeof(intptr_t));
    list->rear = list->front + list->capacity;
    list->capacity = new_cap;
  }
  *list->rear++ = (intptr_t)e;
}

void *list_find(const List *list, bool (*predicate)(const void *, va_list *), ...)
{
  va_list args;
  va_start(args, predicate);
  for (intptr_t *i = list->front; i != list->rear; i++) {
    if (predicate((void *)*i, &args)) {
      va_end(args);
      return (void *)*i;
    }
  }
  va_end(args);
  return NULL;
}

void list_clear(const List *list)
{
  free(list->front);
}
