#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "fileio.h"

int numlength(off_t num) {
    int x;
    while(num) {
        num /= 10;
        ++x;
    }
    return x;
}

off_t fsize(const char *filename) {
    struct stat st;
    if (stat(filename, &st) == 0)
        return st.st_size;
    return -1;
}

char *readfile(char *filename) {
    int c;
    FILE *f;
    off_t filesize;
    char *buffer;
    char *bufferp;

    if ((filesize = fsize(filename)) == -1)
        return NULL;
    buffer = malloc(sizeof(char) * filesize);
    bufferp = buffer;
    if ((f = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "Error opening file: %s", filename);
        return;
    }
    while ((c = getc(f)) != EOF)
        *bufferp++ = c;
    return buffer;
}

char **listdir(char *dir) {
    DIR *dp;
    int length, x;
    char **dirs;
    struct dirent *ep;

    dp = opendir(dir);
    if (dp != NULL) {
        dirs = malloc(sizeof(char*));
        length = 0;
        while (ep = readdir(dp)) {
            dirs = realloc(dirs, sizeof(char*) * length + 1);
            dirs[length++] = strdup(ep->d_name);
        }
        *(dirs + length) = NULL;
        closedir(dp);
        return dirs;
    }
    return NULL;
}

int respond_with_file(int connfd, char* filename) {
	/* not implemented */
	char *status = "HTTP/1.1 %d %s\n";
	status = realloc(status, sizeof(char) * 45);
	free(status);
}

