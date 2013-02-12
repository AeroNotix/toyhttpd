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
    int x = 0;
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
    buffer = malloc(sizeof(char) * (filesize + 1));
    bufferp = buffer;
    if ((f = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return NULL;
    }
    while ((c = getc(f)) != EOF)
        *bufferp++ = c;
    *bufferp = '\0';
    fclose(f);
    return buffer;
}

void freedirlist(char **dirlist) {
    char **dirp;
    dirp = dirlist;
    while (*dirlist) {
        free(*dirlist++);
    }
    free(dirp);
}

char **listdir(char *dir) {
    DIR *dp;
    int length;
    char **dirs = NULL;
    char **olddirs;
    struct dirent *ep;

    dp = opendir(dir);
    if (dp == NULL)
        return NULL;

    length = 1;
    while ((ep = readdir(dp))) {
        dirs = realloc(dirs, sizeof(char*) * length);
        dirs[length-1] = strdup(ep->d_name);
        ++length;
    }
    olddirs = dirs;
    dirs = realloc(dirs, sizeof(char*) * length);
    if (dirs == NULL) {
        freedirlist(olddirs);
        return NULL;
    }
    dirs[length-1] = NULL;
    closedir(dp);
    return dirs;
}

char *generate_index(char *dir) {
    char **dirnames = listdir(dir);
    if (dirnames == NULL) {
        return NULL;
    }
    char **dirnamesp = dirnames;
    char *index;
    char *indexp;
    char *namebuffer;
    int len, maxlen;

    long charcount = 0;
    int dirs = 0;
    maxlen = 0;
    while (*dirnames) {
        ++dirs;
        len = strlen(*dirnames++);
        if (len > maxlen)
            maxlen = len;
        charcount += len;
    }

    index = malloc(sizeof(char) * charcount + (dirs * (30 + maxlen)));
    namebuffer = malloc(sizeof(char) * (13 + maxlen));
    indexp = index;
    if (index == NULL) {
        freedirlist(dirnamesp);
        return NULL;
    }
    dirnames = dirnamesp;
    while (*dirnames) {
        len = strlen(*dirnames);
        sprintf(namebuffer, "<a href=\"/%s\"/>", *dirnames);
        memcpy(index, namebuffer, 13 + len);
        index += (13 + len) ;
        memcpy(index, *dirnames++, len);
        index += len;
        memcpy(index, "</a><br/>", 9);
        index += 9;
    }
    *index = '\0';
    freedirlist(dirnamesp);
    free(namebuffer);
    return indexp;
}

int respond_with_file(int connfd, char* filename) {
    int rval = 0;
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
        rval = -1;
        goto out;
    }
    message_size += strlen(content_length);
    message = malloc(message_size * sizeof(char));
    if (sprintf(message, "%s%s\n%s", status, content_length, content) == -1) {
        rval = -1;
        goto end;
    }
    if (send(connfd, message, strlen(message), 0) == -1) {
        rval = -1;
        goto end;
    }

end:
    free(message);
out:
    free(content_length);
    free(content);
    return rval;
}

int respond_with_string(int connfd, char *string) {
    int rval = 0;
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

    if ((content_length = content_length_line(filesize)) == NULL) {
        return -1;
    }
    message_size += strlen(content_length);
    message = malloc(message_size * sizeof(char));
    if (sprintf(message, "%s%s\n%s", status, content_length, string) == -1) {
        rval = -1;
        goto out;
    }

    if (send(connfd, message, strlen(message), 0) == -1) {
        rval = -1;
        goto out;
    }
out:
    free(message);
    free(content_length);
    return rval;
}

int respond_with_index(int connfd) {
    int rval = 0;
    long message_size;
    char *message;
    char *status;
    char *content_length;
    off_t filesize;
    char *string = generate_index(".");
    if (string == NULL) {
        return respond_with_500(connfd);
    }

    message_size = 0;

    status = "HTTP/1.1 200 OK\n";
    message_size += strlen(status);

    filesize = strlen(string);
    message_size += filesize;
    message_size += numlength(filesize);

    if ((content_length = content_length_line(filesize)) == NULL) {
        rval = -1;
        goto end;
    }

    message_size += strlen(content_length);
    message = malloc(message_size * sizeof(char));
    if (sprintf(message, "%s%s\n%s", status, content_length, string) == -1) {
        rval = -1;
        goto out;
    }

    if (send(connfd, message, strlen(message), 0) == -1) {
        rval = -1;
        goto out;
    }
out:
    free(message);
    free(content_length);
end:
    free(string);
    return rval;
}

char *content_length_line(long number) {
    char* content_length;
    /* 18 is the length of "Content-Length:\n" */
    content_length = malloc((18 + numlength(number)) * sizeof(char));
    if (sprintf(content_length, "Content-Length:%d\n", (int) number) == -1) {
        free(content_length);
        return NULL;
    }
    return content_length;
}

int respond_with_404(int connfd) {
    return respond_with_string(connfd, "<html><body><h3>400 - Not found<h3></body></html>");
}

int respond_with_500(int connfd) {
    return respond_with_string(connfd, "<html><body><h3>500 - Error<h3></body></html>");
}
