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
#include "server.h"

#ifndef SERVER_THREADS
#define SERVER_THREADS 8
#endif

#ifndef REQUEST_LENGTH
#define REQUEST_LENGTH 1024
#endif

socklen_t len[SERVER_THREADS];
struct sockaddr_in6 claddr[SERVER_THREADS];
pthread_t server_threads[SERVER_THREADS];
char request_header[SERVER_THREADS][REQUEST_LENGTH];

struct server_args {
    int sockfd;
    int server_id;
};

int main(void) {
    int sockfd, x;
    struct server_args *sargs;

    /* Bind to our HTTP socket */
    if ((sockfd = socketlisten( /* config */ 12345)) == -1) {
        perror("Couldn't connect to socket");
        exit(1);
    }

    for (x = 0; x < SERVER_THREADS; ++x) {
        sargs = malloc(sizeof(struct server_args));
        sargs->sockfd = sockfd;
        sargs->server_id = x;
        pthread_create(&server_threads[x], NULL, server_loop, sargs);
    }
    for (x = 0; x < SERVER_THREADS; ++x) {
        pthread_join(server_threads[x], NULL);
    }
    return 0;
}

void log_request(struct Request *r) {
    printf("[INFO] %s %s\n",  r->methodline->Method, r->methodline->URL);
}

void* server_loop(void* sargs) {
    int sockfd, server_id, conn;

    sockfd = ((struct server_args*) sargs)->sockfd;
    server_id = ((struct server_args*) sargs)->server_id;
    len[server_id] = sizeof(struct sockaddr_in6);

    /* Server loop */
    while (1) {
        /* clear the buffer between requests */
        memset(&request_header[server_id], 0, REQUEST_LENGTH);
        if ((conn = safe_accept(sockfd, (struct sockaddr*) &claddr[server_id], &len[server_id])) < 0) {
            perror("Error receiving on socket");
            break;
        }

        if (safe_recv(conn, request_header[server_id], REQUEST_LENGTH, 0) < 0) {
            perror("Error receiving on socket");
            break;
        }

        struct MethodLine *ml = readmethodline(request_header[server_id]);
        struct Request *r = malloc(sizeof(struct Request));
        r->methodline = ml;
        r->connfd = conn;
        handle_request(r);
        break;
    }

    free((struct server_args*) sargs);
    return NULL;
}

void* handle_request(void* Request) {
    struct Request *r = Request;
    if (r == NULL) {
        return NULL;
    }
    struct MethodLine *ml = r->methodline;
    log_request(r);
    if (strcmp(ml->URL, "") == 0) {
        if (respond_with_404(r->connfd) < 0) {
            respond_with_500(r->connfd);
        }
    } else if (strcmp(ml->URL, "/") == 0) {
        if (respond_with_index(r->connfd) < 0) {
            respond_with_500(r->connfd);
        }
    } else {
        if (respond_with_file(r->connfd, ml->URL+1) < 0) {
            perror("Error sending on socket");
            respond_with_500(r->connfd);
        }
    }
    freemethodline(ml);
    close(r->connfd);
    free(r);
    return NULL;
}
