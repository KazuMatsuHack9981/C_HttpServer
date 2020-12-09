#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define N 1000
#define MAXLEN 30


int get_filesize(int filefd) {
	int filesize;
	filesize = lseek(filefd, 0, SEEK_END);
	lseek(filefd, 0, SEEK_SET);
	return filesize;
}

void send_header(int client_sockfd, char *type, int size) {
	dprintf(client_sockfd, "HTTP/2.0 200 OK\r\n");
	dprintf(client_sockfd, "Content-Length: %d\r\n", size);
	if(strcmp(type, "json")==0)
		dprintf(client_sockfd, "Content-Type: application/json\r\n");
	else if(strcmp(type, "html")==0)
		dprintf(client_sockfd, "Content-Type: text/html\r\n");
	dprintf(client_sockfd, "\r\n");
}

void redirect(int client_sockfd, char *path) {
	dprintf(client_sockfd, "HTTP/2.0 308 Permanent Redirect\r\n");
	dprintf(client_sockfd, "Location: %s\r\n", path);
	dprintf(client_sockfd, "Content-Length: %d\r\n", 0);
	dprintf(client_sockfd, "\r\n");
}
