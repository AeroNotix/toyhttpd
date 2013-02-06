#include "requests.h"
#include <stdio.h>
#include <string.h>

int main(void) {
    int x, z;
    int cnt;
    char **words;
    char *targets[4] = {
        "IIIHIIII",
        "HHHHHHHH",
        "IIIIIIII"
        "",
        "   H H H"
    };

    while (1) {
        for (x = 0; x < 4; ++x) {
            words = strsplit(targets[x], 'I', &cnt);
            for (z = 0; z < cnt; ++z) {
            }
            freewords(words, cnt);
        }
    }
    return 0;
}
