all:
	gcc -O3 -Wall -Wextra -lpthread request.c fileio.c server.c -o toyhttpd

debug:
	gcc -O0 -g -Wall -Wextra -lpthread request.c fileio.c server.c -o toyhttpd

profile:
	gcc -fprofile-generate -g -Wall -Wextra -lpthread request.c fileio.c server.c -o toyhttpd

profileuse:
	gcc -fprofile-use -g -Wall -Wextra -lpthread request.c fileio.c server.c -o toyhttpd
