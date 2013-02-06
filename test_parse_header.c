#include "requests.h"
#include "hash.c"
#include <stdio.h>
#include <time.h>

void addRandom(struct HashMap *h) {
    int size = rand() % 10;
    char* buf = malloc(sizeof(char) * size);
    int x;
    for (x = 0; x < size; x++) {
        buf[x] = 65 + (rand() % 10);
    }
    printf("%s\n", buf);
    hash_insert(h, buf, buf);
}

int main(void) {
    int x;
    struct HashMap *h;
    char *header = "Wut: InTheButt\nYeah: WutWut";
    char *word;
    char **words;
    char **wordscpy;
//   for (x = 0; x < 1000000; ++x) {
    while (1) {
        h = hash_init();
        for (x = 0; x < 1000; ++x) {
            addRandom(h);
        }
        hash_free(h);
    }
}
