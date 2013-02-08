all:
	gcc -Wall -Wextra -lpthread request.c fileio.c server.c -o toyhttpd

debug:
	gcc -g -Wall -Wextra -lpthread request.c fileio.c server.c -o toyhttpd
