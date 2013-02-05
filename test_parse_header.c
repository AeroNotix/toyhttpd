#include "requests.h"
#include "hash.c"
#include <stdio.h>

int main(void) {
    int x;
    struct HashMap *h;
    char *header = "Wut: InTheButt\nYeah: WutWut";
    char *word;
    char **words;
    char **wordscpy;
    for (x = 0; x < 1000000; ++x) {
        h = hash_init();
        parse_header(header, h);
        hash_free(h);
    }
}
