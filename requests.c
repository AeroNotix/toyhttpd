#include <stdio.h>

#include "hash.h"

struct HashMap* parse_header(char* header) {
    printf("%s\n", header);
    struct HashMap *h = hash_init();
    return h;
}
