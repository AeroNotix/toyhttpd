#ifndef __REQUEST_H
#define __REQUEST_H

#include <sys/socket.h>

struct MethodLine {
    char *Method;
    char *URL;
};

struct Request {
    int connfd;
    struct MethodLine *methodline;
};

struct MethodLine* readmethodline(char *request_header);
void freemethodline(struct MethodLine *ml);
char *method(char* message);
char *requesturl(char* message);

int safe_accept(int socket, struct sockaddr *address, socklen_t *address_len);
ssize_t safe_recv(int socket, void *buffer, size_t length, int flags);
int socketlisten(int port);

#endif // __REQUEST_H
