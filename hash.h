/*
 * An extremely simple and naive implementation of a Hash map.
 * Maps char* to char* and gives allows you to retrieve them.
 */

#ifndef __HASHMAP_H
#define __HASHMAP_H

#include <stdbool.h>

struct Value {
    char *key;
    char *value;
    struct Value *next;
};

struct HashMap {
    int len;
    struct Value **Values;
};

struct HashMap *hash_init();
unsigned long hash(unsigned char *str);
void hash_insert(struct HashMap *h, char* key, char* value);
bool hash_contains(struct HashMap *h, char* key);
char* hash_get(struct HashMap *h, char* key);

void value_free(struct Value *v);
void hash_free(struct HashMap *h);

#endif /* __HASHMAP_H */
