#include <dirent.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "fileio.h"
#include "hash.h"


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

int main(void) {
	int sockfd, conn;
    socklen_t len;
	struct sockaddr_in6 claddr;
	char buffer[1024];

	/* Bind to our HTTP socket */
	if ((sockfd = socketlisten( /* config */ 80)) == -1) {
		perror("Couldn't connect to socket");
		exit(1);
	}

	len = sizeof(struct sockaddr_in6);
	/* Server loop */
	while (1) {
        /* clear the buffer between requests */
        memset(&buffer, 0, 1024);
		if ((conn = accept(sockfd, (struct sockaddr*) &claddr, &len)) < 0) {
			perror("Error receiving on socket");
			break;
		}
		if (recv(conn, buffer, 1024, 0) < 0) {
			perror("Error receiving on socket");
			break;
		}

		if (respond_with_file(conn, "server.c") < 0) {
			perror("Error sending on socket");
			break;
		}
	}

	if (close(sockfd) != 0) {
		perror("Error closing file");
	}
	return 0;
}
