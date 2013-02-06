all:
	gcc -Wall -Wextra fileio.c server.c -o toyhttpd

debug:
	gcc -g -Wall -Wextra fileio.c server.c -o toyhttpd
