/*
 * Filesystem oriented operations.
 */

#ifndef __FILEIO_H
#define __FILEIO_H

#include <sys/types.h>

off_t fsize(const char *filename);
char *readfile(char *filename);
char *generate_index(char *dir);
char **listdir(char *dir);
void freedirlist(char **dirlist);
int respond_with_file(int connfd, char *filename);
int respond_with_string(int connfd, char *string);
int respond_with_index(int connfd);
char *content_length_line(long number);
int respond_with_404(int connfd);
int respond_with_500(int connfd);

#endif // __FILEIO_H
