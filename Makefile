all:
	gcc -Wall -Wextra request.c fileio.c server.c -o toyhttpd

debug:
	gcc -g -Wall -Wextra request.c fileio.c server.c -o toyhttpd
