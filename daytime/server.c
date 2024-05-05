#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
	int sckt = socket(AF_INET, SOCK_STREAM, 0);
	if (sckt < 0) {
		printf("Failed to create socket: %s\n", strerror(errno));
		exit(1);
	}
	struct sockaddr_in addr = {0};
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(420);
	addr.sin_family = AF_INET;

	if (bind(sckt, (struct sockaddr*) &addr, sizeof(struct sockaddr_in)) < 0) {
		printf("Failed to bind to address\n");
		exit(1);
	}

	// Specify the queue of the socket, MAX 10 connections
	if (listen(sckt, 10) < 0) {
		printf("Failed to listen to address\n");
		exit(1);
	}

	char line[] = "Hello, world!\r\n";
	while (1) {
		int ctx = accept(sckt, NULL, NULL);
		if (ctx < 0) continue;
		printf("Accepted a connection!\n");
		if (send(ctx, line, sizeof(line), 0) < 0) {
			printf("Failed to send bytes!\n");
		}
	}
	return 0;
}
