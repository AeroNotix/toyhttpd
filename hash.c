/*
 * An extremely simple and naive implementation of a Hash map.
 * Maps char* to char* and gives allows you to retrieve them.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifndef INITIAL_HASH_SIZE
  #define INITIAL_HASH_SIZE 128
#endif

struct Value {
    struct Value *next;
    char *key;
    char *value;
};

struct HashMap {
    int len;
    struct Value **Values;
};

unsigned long hash(char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++) != '\0') {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

struct HashMap* hash_init() {
    int x;
    struct HashMap *h = malloc(sizeof(struct HashMap));
    if (h == NULL)
        return NULL;

    h->len = INITIAL_HASH_SIZE;
    h->Values = malloc(sizeof(struct Value*) * INITIAL_HASH_SIZE);
    for (x = 0; x < INITIAL_HASH_SIZE; ++x) {
        h->Values[x] = NULL;
    }
    return h;
}

void hash_insert(struct HashMap *h, char *key, char *value) {
    struct Value *slot = h->Values[hash(key) % INITIAL_HASH_SIZE];
    struct Value **slotptr;
    if (slot != NULL) {
        while (slot->next != NULL) {
            slotptr = &slot->next;
            slot = slot->next;
        }
        (*slotptr)->key = strdup(key);
        (*slotptr)->value = strdup(value);
        (*slotptr)->next = NULL;
    } else {
        slot = malloc(sizeof(struct Value));
        slot->key = strdup(key);
        slot->value = strdup(value);
        slot->next = NULL;
        h->Values[hash(key) % INITIAL_HASH_SIZE] = slot;
    }
}

char* hash_get(struct HashMap *h, char* key) {
    struct Value *slot = h->Values[hash(key) % INITIAL_HASH_SIZE];
    while (slot != NULL && strcmp(slot->key, key) != 0) {
        slot = slot->next;
    }
    if (slot != NULL)
        return slot->value;
    else
        return NULL;
}

void value_free(struct Value *v) {
    if (v == NULL) {
        return;
    }
    if (v->next != NULL) {
        value_free(v->next);
    }
    free(v->key);
    free(v->value);
    free(v);
}

void hash_free(struct HashMap *h) {
    int x;
    for (x = 0; x < h->len; x++) {
        value_free(h->Values[x]);
    }
    free(h->Values);
    free(h);
}
