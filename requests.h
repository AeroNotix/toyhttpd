#ifndef __REQUESTS_H
#define __REQUESTS_H

struct HashMap;
void parse_header(char* header, struct HashMap *h);
void freewords(char **words, int cnt);
char **strsplit(char *string, char sep, int *realcnt);
#endif /* __REQUESTS_H */
