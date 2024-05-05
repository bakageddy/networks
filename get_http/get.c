#include <stdint.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	if (argc < 3) {
		printf("Usage: ./get [HTTP URL] [PAYLOAD]\n");
		exit(1);
	}

	struct addrinfo hints = {0};
	struct addrinfo *res;

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = 0;
	hints.ai_protocol = 0;

	const char *url = argv[1];
	int s = 0;
	if ((s = getaddrinfo(url, "80", &hints, &res)) != 0) {
		printf("Failed to get address info: %s\n", gai_strerror(s));
		exit(1);
	}

	int sckt = 0;

	for (struct addrinfo *rp = res; rp != NULL; rp = rp->ai_next) {
		sckt = socket(rp -> ai_family, rp -> ai_socktype, rp -> ai_protocol);
		if (sckt == -1) {
			printf("Failed to create socket!\n");
			exit(1);
		}
		if (connect(sckt, rp -> ai_addr, sizeof(*rp -> ai_addr)) < 0) {
			printf("Failed to connect to network!\n");
		} else break;
	}
	freeaddrinfo(res);
	// Dinesh was here!

	char payload[1024];
	sprintf(payload, "GET /%s HTTP/1.1\r\n\r\n", argv[2]);
	if (send(sckt, payload, sizeof(payload) + 1, 0) < 0) {
		printf("Failed to send message!");
	}

	char msg[1024];
	while (read(sckt, msg, sizeof(msg)) > 0) {
		printf("%s", msg);
	}

	printf("\n");
	return 0;
}
