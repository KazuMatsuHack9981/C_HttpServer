#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXPENDING 5

typedef struct sockaddr sockadd;
typedef struct sockaddr_in sockadd_in;

void HandleTCPClient(int client_sockfdet);

int main(int argc, char *argv[]) {
	int server_sockfd;
	int client_sockfd;
	sockadd_in server_addr;
	sockadd_in client_addr;
	unsigned short port;
	unsigned int clntLen;

	if (argc!=2) {
		fprintf(stderr, "Usage:  %s <port>\n", argv[0]);
		return 1;
	}

  port = atoi(argv[1]);
	server_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(port);

	int check = bind(server_sockfd, (sockadd *) &server_addr, sizeof(server_addr));
	if(check!=0) {
		fprintf(stderr, "Bind failed (check if port is available)\n");
		return 1;
	}

	listen(server_sockfd, MAXPENDING) < 0;
	printf("listening...\n");
	
	while(1) {
		clntLen = sizeof(client_addr);
		client_sockfd = accept(server_sockfd, (sockadd *) &client_addr, &clntLen);
		printf("Handling client %s\n", inet_ntoa(client_addr.sin_addr));
		HandleTCPClient(client_sockfd);
		close(client_sockfd);
	}
}
