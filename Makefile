all:
	gcc -Wall -Wextra hash.c fileio.c server.c -o toyhttpd
