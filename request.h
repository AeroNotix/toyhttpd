#ifndef __REQUEST_H
#define __REQUEST_H

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
int socketlisten(int port);

#endif // __REQUEST_H
