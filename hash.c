/*
 * An extremely simple and naive implementation of a Hash map.
 * Maps char* to char* and gives allows you to retrieve them.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

#ifndef INITIAL_HASH_SIZE
  #define INITIAL_HASH_SIZE 128
#endif


unsigned long hash(char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

struct HashMap* InitHashMap() {
    struct HashMap *h = malloc(sizeof(struct HashMap));
    if (h == NULL)
        return NULL;

    h->len = INITIAL_HASH_SIZE;
    h->Values = malloc(sizeof(struct Value*) * INITIAL_HASH_SIZE);
    return h;
}

void Insert(struct HashMap *h, char* key, char* value) {
    struct Value *slot = h->Values[hash(key) % INITIAL_HASH_SIZE];
    while (slot != NULL) {
        slot = slot->next;
    }
    slot = malloc(sizeof(struct Value));
    slot->key = key;
    slot->value = value;
    slot->next = NULL;
    h->Values[hash(key) % INITIAL_HASH_SIZE] = slot;
}

bool Contains(struct HashMap *h, char* key) {
    struct Value *slot = h->Values[hash(key) % INITIAL_HASH_SIZE];
    if (slot == NULL)
        return false;
    while (slot != NULL && strcmp(slot->key, key) != 0) {
        slot = slot->next;
    }

    return !(slot == NULL || slot->value == NULL);
}

char* Get(struct HashMap *h, char* key) {
    if (Contains(h, key)) {
        struct Value *slot = h->Values[hash(key) % INITIAL_HASH_SIZE];
        while (slot != NULL && strcmp(slot->key, key) != 0) {
            slot = slot->next;
        }
        return slot->value;
    } else {
        return "";
    }
}

void ValueFree(struct Value *v) {
    if (v == NULL)
        return;
    if (v->next != NULL)
        ValueFree(v->next);
    free(v);
}

void HashFree(struct HashMap *h) {
    int x;
    for (x = 0; x < h->len; x++) {
        ValueFree(h->Values[x]);
    }
}
