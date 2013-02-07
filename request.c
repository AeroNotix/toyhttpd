#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "request.h"

struct MethodLine;

struct MethodLine *readmethodline(char *request_header) {
    struct MethodLine *ml = malloc(sizeof(struct MethodLine));
    ml->Method = method(request_header);
    ml->URL = requesturl(request_header);
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
    buffer = malloc(sizeof(char*) * 7);
    bufferptr = buffer;
    while(*message != ' ' && *message != '\0') {
        *buffer++ = *message++;
    }
    *buffer = '\0';
    return bufferptr;
}

char *requesturl(char* message) {
    int urllength;
    char *urlstart;
    char *urlend;
    char *buffer;

    urlstart = message;
    while (*urlstart++ != ' ' && *urlstart != '\0');
    urlend = urlstart;
    while (*urlend++ != ' ' && *urlend != '\0') {
        ++urllength;
    }

    buffer = malloc(sizeof(char*) * (urllength + 1));
    memcpy(buffer, urlstart, urllength);
    buffer[urllength] = '\0';
    return buffer;
}
