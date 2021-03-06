#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#include "request.h"

struct MethodLine;
static pthread_mutex_t wmutex = PTHREAD_MUTEX_INITIALIZER;

struct MethodLine *readmethodline(char *request_header) {
    struct MethodLine *ml = malloc(sizeof(struct MethodLine));
    ml->Method = method(request_header);
    if (ml->Method == NULL) {
        ml->Method = strdup("");
    }
    ml->URL = requesturl(request_header);
    if (ml->URL == NULL) {
        ml->URL = strdup("");
    }
    return ml;
}

void freemethodline(struct MethodLine *ml) {
    free(ml->Method);
    free(ml->URL);
    free(ml);
}

char *method(char* message) {
    char *buffer;
    char *bufferptr;

    /*
     * 7 is the maximum METHOD length as per:
     * http://www.w3.org/Protocols/rfc2616/rfc2616-sec9.html
     */
    buffer = malloc(sizeof(char) * 7);
    if (buffer == NULL) {
        return NULL;
    }
    bufferptr = buffer;
    while(*message != ' ' && *message != '\0') {
        *buffer++ = *message++;
    }
    *buffer = '\0';
    return bufferptr;
}

char *requesturl(char* message) {
    int urllength = 0;
    char *urlstart;
    char *urlend;
    char *buffer = NULL;

    urlstart = message;
    while (*urlstart++ != ' ' && *urlstart != '\0');
    urlend = urlstart;
    while (*urlend++ != ' ' && *urlend != '\0') {
        ++urllength;
    }

    buffer = malloc(sizeof(char) * (urllength + 1));
    if (buffer == NULL) {
        return NULL;
    }
    memcpy(buffer, urlstart, urllength);
    buffer[urllength] = '\0';
    return buffer;
}

inline int safe_accept(int socket, struct sockaddr *address,
                socklen_t *address_len) {
    int conn;
    pthread_mutex_lock(&wmutex);
    conn = accept(socket, address, address_len);
    pthread_mutex_unlock(&wmutex);
    return conn;
}

inline ssize_t safe_recv(int socket, void *buffer,
                  size_t length, int flags) {
    ssize_t len;
    len = recv(socket, buffer, length, flags);
    return len;
}

int socketlisten(int port) {
    int sockfd;
    struct sockaddr_in serv_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        return -1;
    }
    int yes = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    memset(&serv_addr, 0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) {
        return -1;
    }
    if (listen(sockfd, 5) < 0) {
        return -1;
    }
    return sockfd;
}
