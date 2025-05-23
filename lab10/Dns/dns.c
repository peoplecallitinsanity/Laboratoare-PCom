// Protocoale de comunicatii
// Laborator 10 - DNS
// dns.c

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

int usage(char* name)
{
	printf("Usage:\n\t%s -n <NAME>\n\t%s -a <IP>\n", name, name);
	return 1;
}

// Receives a name and prints IP addresses
void get_ip(char* name)
{
	int ret;
	struct addrinfo hints, *result, *p;

	// TODO: set hints
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	// TODO: get addresses
	ret = getaddrinfo(name, NULL, &hints, &result);
	if (ret != 0) {
		return;
	}

	// TODO: iterate through addresses and print them
	for (p = result; p != NULL; p = p->ai_next) {
		void *adresa;
		char ip_text[INET6_ADDRSTRLEN];

		if (p->ai_family == AF_INET) {
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
			adresa = &(ipv4->sin_addr);
		} else {
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
			adresa = &(ipv6->sin6_addr);
		}

		inet_ntop(p->ai_family, adresa, ip_text, sizeof(ip_text));
		printf("Adresa: %s\n", ip_text);
	}

	// TODO: free allocated data
	freeaddrinfo(result);
}

// Receives an address and prints the associated name and service
void get_name(char* ip)
{
	int ret;
	struct sockaddr_in addr;
	char host[1024];
	char service[20];

	// TODO: fill in address data
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(0);
	inet_aton(ip, &addr.sin_addr);

	// TODO: get name and service
	ret = getnameinfo((struct sockaddr *)&addr, sizeof(addr),
	                  host, sizeof(host),
	                  service, sizeof(service), 0);
	if (ret != 0) {
		return;
	}

	// TODO: print name and service
	printf("Nume: %s\n", host);
	printf("Serviciu: %s\n", service);
}

int main(int argc, char **argv)
{
	if (argc < 3) {
		return usage(argv[0]);
	}

	if (strncmp(argv[1], "-n", 2) == 0) {
		get_ip(argv[2]);
	} else if (strncmp(argv[1], "-a", 2) == 0) {
		get_name(argv[2]);
	} else {
		return usage(argv[0]);
	}

	return 0;
}
