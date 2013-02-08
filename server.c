#include <dirent.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#include "fileio.h"
#include "request.h"

#ifndef REQUEST_LENGTH
#define REQUEST_LENGTH 1024
#endif

void* handle_request(void *Request);

int main(void) {
    int sockfd, conn;
    socklen_t len;
    struct sockaddr_in6 claddr;
    pthread_t t;
    char request_header[REQUEST_LENGTH];

    /* Bind to our HTTP socket */
    if ((sockfd = socketlisten( /* config */ 12345)) == -1) {
        perror("Couldn't connect to socket");
        exit(1);
    }

    len = sizeof(struct sockaddr_in6);
    /* Server loop */
    while (1) {
        /* clear the buffer between requests */
        memset(&request_header, 0, REQUEST_LENGTH);
        if ((conn = accept(sockfd, (struct sockaddr*) &claddr, &len)) < 0) {
            perror("Error receiving on socket");
            break;
        }
        if (recv(conn, request_header, REQUEST_LENGTH, 0) < 0) {
            perror("Error receiving on socket");
            break;
        }

        struct MethodLine *ml = readmethodline(request_header);
        struct Request *r = malloc(sizeof(struct Request));
        r->methodline = ml;
        r->connfd = conn;
        pthread_create(&t, NULL, handle_request, r);
        pthread_join(t, NULL);
    }

    if (close(sockfd) != 0) {
        perror("Error closing file");
    }
    return 0;
}

void log_request(struct Request *r) {
    printf("[INFO] %s %s\n",  r->methodline->Method, r->methodline->URL);
}

void* handle_request(void* Request) {
    struct Request *r = Request;
    if (r == NULL) {
        return NULL;
    }
    struct MethodLine *ml = r->methodline;
    log_request(r);
    if (strcmp(ml->URL, "/") == 0) {
        if (respond_with_index(r->connfd) < 0) {
            respond_with_string(r->connfd, "<html><body><h3>500 - Error<h3></body></html>");
        }
    } else {
        if (respond_with_file(r->connfd, ml->URL+1) < 0) {
            perror("Error sending on socket");
            respond_with_string(r->connfd, "<html><body><h3>500 - Error<h3></body></html>");
        }
    }

    freemethodline(ml);
    close(r->connfd);
    free(r);
    return NULL;
}
