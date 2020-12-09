#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/select.h>
#define N 10000

typedef struct addrinfo addrinf;

int main(int argc, char **argv) {
	ssize_t  n;
	int      sock_fd;
	fd_set   read_fd;
	addrinf  preaddr;
	addrinf  *addr;

	if(argc!=3) {
		fprintf(stderr, "Usage: %s <ip> <port>\n", argv[0]);
		return 1;
	}

	memset(&preaddr, 0, sizeof(preaddr));
	preaddr.ai_family   = AF_INET;
	preaddr.ai_socktype = SOCK_STREAM;

	getaddrinfo(argv[1], argv[2], &preaddr, &addr);
	sock_fd = socket(addr->ai_family, addr->ai_socktype, 0);
	connect(sock_fd, addr->ai_addr, addr->ai_addrlen);

	while(1) {
		FD_ZERO(&read_fd);
		FD_SET(0, &read_fd);
		FD_SET(sock_fd, &read_fd);

		select(sock_fd+1, &read_fd, NULL, NULL, NULL);

		if(FD_ISSET(0, &read_fd)) {
			char buf[N];
			n = read(0, buf, sizeof(buf));
			if(n==0) break;
			write(sock_fd, buf, n);
		}

		if(FD_ISSET(sock_fd, &read_fd)) {
			char buf[N];
			n = read(sock_fd, buf, sizeof(buf));
			if(n==0) break;
			write(1, buf, n);
		}
	}

	shutdown(sock_fd, SHUT_RDWR);
	return 0;
}
