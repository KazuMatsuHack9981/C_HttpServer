#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

typedef struct tm TIME;

void getdate(void) {
	int jsonfd;
  time_t t = time(NULL);
  TIME tm  = *localtime(&t);

	jsonfd = open("buf.json", O_WRONLY|O_CREAT|O_TRUNC, 0666);
	dprintf(jsonfd, "{\r\n");
	dprintf(jsonfd, "\t\"date\": \"%d-%02d-%02d %02d:%02d:%02d\"\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	dprintf(jsonfd, "}\r\n");

	close(jsonfd);
}

void add(char **queries) {
	int jsonfd;
	int a = atoi(queries[0]);
	int b = atoi(queries[1]);
	jsonfd = open("buf.json", O_WRONLY|O_CREAT|O_TRUNC, 0666);
	dprintf(jsonfd, "{\r\n");
	dprintf(jsonfd, "\t\"result\": \"%d\",", a+b);
	dprintf(jsonfd, "\t\"comment\": \"try with other queries!\"");
	dprintf(jsonfd, "}\r\n");
	close(jsonfd);
}
