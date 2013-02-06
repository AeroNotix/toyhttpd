#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/socket.h>
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
        fprintf(stderr, "Error opening file: %s\n", filename);
        return NULL;
    }
    while ((c = getc(f)) != EOF)
        *bufferp++ = c;
    fclose(f);
    return buffer;
}

char **listdir(char *dir) {
    DIR *dp;
    int length;
    char **dirs;
    struct dirent *ep;

    dp = opendir(dir);
    if (dp != NULL) {
        dirs = malloc(sizeof(char*));
        length = 0;
        while ((ep = readdir(dp))) {
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
    long message_size;
    char *message;
    char *status;
    char *content_length;
    char *content;
    off_t filesize;

    message_size = 0;

    status = "HTTP/1.1 200 OK\n";
    message_size += strlen(status);

    if ((filesize = fsize(filename)) == -1) {
        return -1;
    }
    if ((content = readfile(filename)) == NULL) {
        return -1;
    }
    message_size += strlen(content);
    message_size += numlength(filesize);

    /* 18 is the length of "Content-Length:\n" */
    content_length = malloc((18 + numlength(filesize)) * sizeof(char));
    if (sprintf(content_length, "Content-Length:%d\n", (int)filesize-1) == -1) {
        goto out;
    }
    message_size += strlen(content_length);
    message = malloc(message_size * sizeof(char));
    if (sprintf(message, "%s%s\n%s", status, content_length, content) == -1) {
        goto end;
    }
    if (send(connfd, message, strlen(message), 0) == -1) {
        goto end;
    }

    free(content_length);
    free(content);
    free(message);
    return 0;
end:
    free(message);
out:
    free(content_length);
    free(content);
    return -1;
}

int respond_with_string(int connfd, char *string) {
    long message_size;
    char *message;
    char *status;
    char *content_length;
    off_t filesize;

    message_size = 0;

    status = "HTTP/1.1 200 OK\n";
    message_size += strlen(status);

    filesize = strlen(string);
    message_size += filesize;
    message_size += numlength(filesize);

    /* 18 is the length of "Content-Length:\n" */
    content_length = malloc((18 + numlength(filesize)) * sizeof(char));
    if (sprintf(content_length, "Content-Length:%d\n", (int)filesize) == -1) {
        goto end;
    }
    message_size += strlen(content_length);
    message = malloc(message_size * sizeof(char));
    if (sprintf(message, "%s%s\n%s", status, content_length, string) == -1) {
    }

    if (send(connfd, message, strlen(message), 0) == -1) {
        goto out;
    }
    return 0;
end:
    free(message);
out:
    free(content_length);
    return -1;

}
