#ifndef UTIL_HASHTABLE_H
#define UTIL_HASHTABLE_H

#include "util/list.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct HashTable HashTable;
struct HashTable {
  size_t capacity;
  List *table;
  size_t (*hash)(const void *);
  bool (*matcher)(const void *, const void *);
};

void hashtable_init(HashTable *hashtable, size_t capacity, size_t (*hash)(const void *),
                    bool (*matcher)(const void *, const void *));

void *hashtable_get(const HashTable *hashtable, const void *key);

void hashtable_put(HashTable *hashtable, void *key, void *value);

#endif
