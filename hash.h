/*
 * An extremely simple and naive implementation of a Hash map.
 * Maps char* to char* and gives allows you to retrieve them.
 */

#ifndef __HASHMAP_H
#define __HASHMAP_H

#include <stdbool.h>

struct HashMap;
struct Value;

struct HashMap *InitHashMap();
unsigned long hash(unsigned char *str);
void Insert(struct HashMap *h, char* key, char* value);
bool Contains(struct HashMap *h, char* key);
char* Get(struct HashMap *h, char* key);

void ValueFree(struct Value *v);
void HashFree(struct HashMap *h);

#endif /* __HASHMAP_H */
