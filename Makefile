all:
	gcc -Wall -Wextra hash.c requests.c fileio.c server.c -o toyhttpd
