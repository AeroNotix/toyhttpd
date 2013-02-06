#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash.h"

char **strsplit(char *string, char sep, int *realcnt) {
    /*
      Will allocate `realcnt` strings. You need to clear this memory
      yourself.

      Will allocate an array to hold the strings in. You need to clear
      this memory yourself.
    */

    int x;
    int cnt;
	int parts;
    int wordcnt = 0;

	char **words;
	char **wordscpy;
    char *strptr;

    strptr = string;
    parts = 1;

	/* Tally up and malloc the maximum splits */
	for (x = 0; *strptr; ++strptr, ++x) {
		if (*strptr == sep) {
			parts++;
        }
	}

    /* `x` will be the strlength if it's 0 then it's string is the
       empty string */
    if (x == 0) {
        *realcnt = 0;
        return NULL;
    }
    
	words = malloc(sizeof(char*) * parts);
	if (words == NULL) {
		return NULL;
	}

    /* If `parts` is still 1, it means the separator was not found */
    if (parts == 1) {
        *realcnt = 1;
        *words = strdup(string);
        return words;
    }

    wordscpy = words;
    strptr = string;
    while(*strptr != '\0') {
        cnt = 0;
        while(*strptr != sep && *strptr != '\0') {
            if (cnt == 0) {
                *words = malloc(sizeof(char));
            }
            *words = realloc(*words, sizeof(char) * cnt+1);
            (*words)[cnt++] = *strptr++;
        }
        ++strptr;
        /* If cnt is greater than zero and we're here then it means
           we've actually copied a string into *words. We need to
           null terminate it and increment the word pointer. */
        if (cnt > 0) {
            (*words)[cnt] = '\0';
            ++wordcnt;
            ++words;
        }
    }

    *realcnt = wordcnt;
    return wordscpy;
}

char **parse_key(char *header_pair) {
    int x;
    int cidx;
    char *headerptr;
    char **keypair;

    cidx = -1;
    headerptr = header_pair;
    for (x = 0; *headerptr != '\0'; ++headerptr, ++x) {
        /* If we've found ':' and we've not seen one before */
        if (*headerptr == ':' && cidx == -1) {
            cidx = x;
        }
    }
    if (cidx == -1) {
        return NULL;
    }
    keypair = malloc(sizeof(char*) * 2);
    if (keypair == NULL) {
        return NULL;
    }
    keypair[0] = malloc(sizeof(char) * cidx + 1);
    keypair[1] = malloc(sizeof(char) * (x - cidx));
    memcpy(keypair[0], header_pair, cidx);
    memcpy(keypair[1], header_pair+cidx+2, (x - cidx));
    keypair[0][cidx] = '\0';
    keypair[1][x - cidx] = '\0';
    return keypair;
}

void freewords(char **words, int cnt) {
    int x;
    for (x = 0; x < cnt; ++x) {
        free(words[x]);
    }
    free(words);    
}

void parse_header(char* header, struct HashMap *h) {
    int cnt;
    int x;
    int inner;
    char **entries;
    char **subentries;

    cnt = 0;
    entries = strsplit(header, '\n', &cnt);
    for (x = 0; x < cnt; ++x) {
        inner = 0;
        if ((subentries = strsplit(entries[x], ':', &inner)) != NULL) {
            if (inner < 2) {
                freewords(subentries, inner);
                continue;
            }
            hash_insert(h, subentries[0], subentries[1]);
            free(subentries[0]);
            free(subentries[1]);
            free(subentries);
        }
    }
    freewords(entries, cnt);
}
