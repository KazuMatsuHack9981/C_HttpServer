#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/fcntl.h>
#include <unistd.h>

#define RCVBUFSIZE 10000
#define N 500
#define MAXQUERY 200

// HttpAssets
int get_filesize(int filefd);
void send_header(int client_sockfd, char *type, int size);
void redirect(int client_sockfd, char *path);

// Lambda functions
void getdate(void);
void add(char **queries);


void HandleTCPClient(int client_sockfd) {
	char method[N];
	char uri[N];
	char *uri_path;
	char uri_path_copy[N];
	char version[N];
	char message[N];
	int  filefd;
	int  i, len;
	char buf[RCVBUFSIZE];
	char *filename;
	char *prefix;
	char *queries[MAXQUERY];

	//read client request
	read(client_sockfd, buf, RCVBUFSIZE);
	sscanf(buf, "%s %s %s", method, uri, version);

	if(strcmp(method, "GET") != 0)
		dprintf(client_sockfd, "Only GET is supported");


	// handle root GET
	if(strcmp(uri, "/")==0) {
		redirect(client_sockfd, "/index.html");
		return;
	}

	//evaluate requested uri path and prefix
	uri_path = uri+1;
	strcpy(uri_path_copy, uri_path);

	// get prefix
	filename = strtok(uri_path_copy, ".");
	prefix = strtok(NULL, "?");

	// get querystrings
	for(i=0; 1; i++) {
		if( (queries[i]=strtok(NULL, "="))==NULL )
			break;
		queries[i] = strtok(NULL, "&");
	}


	if(strcmp(prefix, "html") == 0) {
		if((filefd = open(uri_path, O_RDONLY, 0666)) == -1)
			dprintf(client_sockfd, "404 Not Found");
		else {
			send_header(client_sockfd, "html", get_filesize(filefd));
			while( (len=read(filefd, buf, RCVBUFSIZE)) > 0 )
				write(client_sockfd, buf, len);
		}
	}
	else if(strcmp(prefix, "api") == 0) {
		// date api
		if(strcmp(filename, "date")==0) {
			getdate();
			if((filefd = open("buf.json", O_RDONLY, 0666)) == -1)
				dprintf(client_sockfd, "404 API Not Found");
			else {
				send_header(client_sockfd, "json", get_filesize(filefd));
				while( (len=read(filefd, buf, RCVBUFSIZE)) > 0 )
					write(client_sockfd, buf, len);
			}
		}

		// add api
		if(strcmp(filename, "add")==0) {
			if(queries[0]==NULL || queries[1]==NULL) {
				dprintf(client_sockfd, "400 Bad Query\n");
				dprintf(client_sockfd, "try /add.api?a=1&b=2");
			}
			else {
				add(queries);
				if((filefd = open("buf.json", O_RDONLY, 0666)) == -1)
					dprintf(client_sockfd, "404 API Not Found");
				else {
					send_header(client_sockfd, "json", get_filesize(filefd));
					while( (len=read(filefd, buf, RCVBUFSIZE)) > 0 )
						write(client_sockfd, buf, len);
				}
			}
		}
	}
}
