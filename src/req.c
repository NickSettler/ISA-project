/**
 * @author Nikita Moiseev<xmoise01@stud.fit.vutbr.cz>
 * @file: request.c
 * @date: 18.11.2023
 */

#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#include "res.h"
#include "req.h"

/**
 * Set DNS request headers
 * @param config configuration of program
 * @param buffer buffer where to store headers
 */
void setHeaders(config_t *config, char *buffer) {
    dns_header_t *dns = (dns_header_t *) buffer;

    dns->id = (unsigned short) htons(getpid());
    dns->flags = config->recursive;
    dns->question = htons(1);
    dns->answer = 0;
    dns->authority = 0;
    dns->additional = 0;
}

/**
 * Open socket for communication
 * @param config configuration of program
 * @return socket descriptor
 */
int openSocket(config_t *config) {
    int s = -1, mode = AF_INET;

    struct sockaddr_in server;
    struct addrinfo hints, *dnsProvider = NULL;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if (getaddrinfo(config->host, NULL, &hints, &dnsProvider) != 0) {
        freeaddrinfo(dnsProvider);
        perror("Cannot get DNS provider");
        exit(EXIT_FAILURE);
    }

    for (struct addrinfo *i = dnsProvider; i != NULL; i = i->ai_next) {
        char str[INET6_ADDRSTRLEN];
        memset(str, 0, sizeof(str));

        server.sin_addr.s_addr =
                i->ai_addr->sa_family == AF_INET
                        ? inet_addr(
                                  inet_ntop(AF_INET, &((struct sockaddr_in *) i->ai_addr)->sin_addr, str, sizeof(str)))
                        : inet_addr(inet_ntop(AF_INET6, &((struct sockaddr_in6 *) i->ai_addr)->sin6_addr, str,
                                              sizeof(str)));

        s = socket(mode, SOCK_DGRAM, 0);
        if (s == -1) continue;

        server.sin_family = i->ai_addr->sa_family == AF_INET ? AF_INET : AF_INET6;
        server.sin_port = htons(config->port);

        if (connect(s, (struct sockaddr *) &server, sizeof(server)) < 0) continue;

        break;
    }

    freeaddrinfo(dnsProvider);

    if (s == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    return s;
}

/**
 * Send DNS request and receive response
 * @param socket socket descriptor
 * @param config configuration of program
 */
void request(int socket, config_t *config) {
    char req[DNS_PACKET_BUFF_SIZE], resp[DNS_PACKET_BUFF_SIZE];

    setHeaders(config, req);

    size_t domainLength = config->reverseLookup ? writeQueryPtr(config, (char *) &req[sizeof(dns_header_t)])
                                                : writeQueryDomain(config->addr, &req[sizeof(dns_header_t)]);

    writeQueryType(config, (unsigned short *) &req[sizeof(dns_header_t) + domainLength]);

    sendDNSRequest(socket, req, domainLength);

    getDnsResponse(socket, resp);

    close(socket);

    printDnsResponse(resp, domainLength);
}

/**
 * Receive DNS response
 * @param socket socket descriptor
 * @param buffer buffer to store response
 */
void getDnsResponse(int socket, char *buffer) {
    if (recv(socket, (char *) buffer, DNS_PACKET_BUFF_SIZE, 0) < 0) {
        perror("recv call failed");
        exit(EXIT_FAILURE);
    }
}

/**
 * Send DNS request to host
 * @param socket socket descriptor
 * @param buffer buffer with request
 * @param domainLen length of domain
 */
void sendDNSRequest(int socket, const char *buffer, size_t domainLen) {
    size_t totalSize = sizeof(dns_header_t) + domainLen + sizeof(uint16_t) * 2;

    if (send(socket, (const char *) buffer, totalSize, 0) < 0) {
        perror("send call failed");
        exit(EXIT_FAILURE);
    }
}

/**
 * Write query type
 * @param config configuration of program
 * @param buffer buffer where to store query type
 */
void writeQueryType(config_t *config, unsigned short *buffer) {
    unsigned short queryType = config->reverseLookup ? 12 : (config->ipv6 ? 28 : 1);
    *buffer++ = htons(queryType);
    *buffer = htons(1);
}

/**
 * Write query for forward lookup
 * @param domain domain name
 * @param queryName buffer to store query
 * @return size of stored query
 */
size_t writeQueryDomain(char *domain, char *queryName) {
    size_t ret = 1;

    char *token = strtok(domain, ".");
    while (token != NULL) {
        size_t tokenLength = strlen(token);

        *queryName++ = (unsigned char) tokenLength;
        memcpy(queryName, token, tokenLength);
        queryName += tokenLength;

        ret += (tokenLength + 1);

        token = strtok(NULL, ".");
    }

    *queryName++ = '\0';

    return ret;
}

/**
 * Write query for reverse lookup
 * @param config configuration of program
 * @param queryName buffer to store query
 * @return size of stored query
 */
size_t writeQueryPtr(config_t *config, char *queryName) {
    char buffer[DNS_PACKET_BUFF_SIZE];

    if (config->reverseIPv6) {
        sprintf(queryName, "%s", reverseIPv6(config->addr));
        return 74;
    } else {
        snprintf(buffer, DNS_PACKET_BUFF_SIZE, "%s.in-addr.arpa", reverseIP(config->addr));
        return writeQueryDomain(buffer, queryName);
    }
}

/**
 * Reverse IP address
 * @param ip IP address in format x.x.x.x
 * @return reversed IP address
 */
char *reverseIP(char *ip) {
    struct in_addr addr;

    if (inet_pton(AF_INET, ip, &addr) != 1) {
        perror("inet_pton failed");
        exit(EXIT_FAILURE);
    }

    addr.s_addr = htonl(addr.s_addr);

    if (inet_ntop(AF_INET, &addr, (char *) ip, INET_ADDRSTRLEN) == NULL) {
        perror("inet_ntop failed");
        exit(EXIT_FAILURE);
    }

    return (char *) ip;
}

/**
 * Reverse IPv6 address
 * @return reversed IPv6 address
 */
char *reverseIPv6(char *ipv6Address) {
    struct in6_addr reversed;
    char reverseDns[INET6_ADDRSTRLEN + 32];

    if (inet_pton(AF_INET6, ipv6Address, &reversed) != 1) {
        perror("inet_pton failed");
        exit(EXIT_FAILURE);
    }

    char *dnsPointer = reverseDns;
    for (int i = 15; i >= 0; i--) {
        sprintf(dnsPointer, "\01%01x\01%01x", reversed.s6_addr[i] & 0x0f, (reversed.s6_addr[i] & 0xf0) >> 4);
        dnsPointer += 4;
    }

    strcpy(dnsPointer, "\03ip6\4arpa\0");

    if (strcpy((char *) ipv6Address, reverseDns) == NULL) exit(EXIT_FAILURE);

    return (char *) ipv6Address;
}
