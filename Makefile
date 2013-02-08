all:
	gcc -Wall -Wextra -lpthread request.c fileio.c server.c -o toyhttpd

debug:
	gcc -g -Wall -Wextra -lpthread request.c fileio.c server.c -o toyhttpd

profile:
	gcc -fprofile-generate -g -Wall -Wextra -lpthread request.c fileio.c server.c -o toyhttpd

profileuse:
	gcc -fprofile-use -g -Wall -Wextra -lpthread request.c fileio.c server.c -o toyhttpd
