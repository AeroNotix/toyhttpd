#ifndef __SERVER_H
#define __SERVER_H

#include "request.h"

void* server_loop(void* sargs);
void* handle_request(void* Request);

#endif /* __SERVER_H */
