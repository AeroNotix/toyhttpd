/*
 * Filesystem oriented operations.
 */

#ifndef __FILEIO_H
#define __FILEIO_H

off_t fsize(const char *filename);
char *readfile(char *filename);
char **listdir(char *dir);
int respond_with_file(int connfd, char* filename);

#endif // __FILEIO_H
