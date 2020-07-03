#include "util/hashtable.h"

#include <stdarg.h>
#include <stdlib.h>

typedef struct HTBucket HTBucket;
struct HTBucket {
  void *key;
  void *value;
};

static bool match(const void *bucket, va_list *args)
{
  va_list va;
  va_copy(va, *args);
  HashTable *hashtable = va_arg(va, HashTable *);
  void *key = va_arg(va, void *);
  va_end(va);
  return hashtable->matcher(((HTBucket *)bucket)->key, key);
}

void hashtable_init(HashTable *hashtable, size_t capacity, size_t (*hash)(const void *),
                    bool (*matcher)(const void *, const void *))
{
  hashtable->capacity = capacity;
  hashtable->table = calloc(capacity, sizeof(List));
  for (size_t i = 0; i < capacity; i++) {
    list_init(&hashtable->table[i], 0);
  }
  hashtable->hash = hash;
  hashtable->matcher = matcher;
}

void *hashtable_get(const HashTable *hashtable, const void *key)
{
  size_t index = hashtable->hash(key) % hashtable->capacity;
  HTBucket *bucket = list_find(&hashtable->table[index], match, hashtable, key);
  return bucket == NULL ? NULL : bucket->value;
}

void hashtable_put(HashTable *hashtable, void *key, void *value)
{
  size_t index = hashtable->hash(key) % hashtable->capacity;
  HTBucket *bucket = malloc(sizeof(HTBucket));
  bucket->key = key;
  bucket->value = value;
  list_add(&hashtable->table[index], bucket);
}
