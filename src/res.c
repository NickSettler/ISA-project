/**
 * @author Nikita Moiseev<xmoise01@stud.fit.vutbr.cz>
 * @file: response.c
 * @date: 18.11.2023
 */

#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "res.h"

/**
 * Get DNS type.
 * @param type DNS type.
 * @return DNS type string.
 */
char *getDNSType(uint16_t type) {
    switch (ntohs(type)) {
        case A_REC:
            return A_REC_TYPE;
        case NS_REC:
            return NS_REC_TYPE;
        case CNAME_REC:
            return CNAME_REC_TYPE;
        case SOA_REC:
            return SOA_REC_TYPE;
        case PTR_REC:
            return PTR_REC_TYPE;
        case AAAA_REC:
            return AAAA_REC_TYPE;
        default:
            return UNKNOWN_TYPE;
    }
}

/**
 * Get DNS class.
 * @param cls DNS class.
 * @return DNS class string.
 */
char *getDNSClass(uint16_t cls) {
    switch (ntohs(cls)) {
        case IN_DNS_CODE:
            return IN_DNS_TYPE;
        case CS_DNS_CODE:
            return CS_DNS_TYPE;
        case CH_DNS_CODE:
            return CH_DNS_TYPE;
        case HS_DNS_CODE:
            return HS_DNS_TYPE;
        default:
            return UNKNOWN_TYPE;
    }
}

/**
 * Extract domain name from DNS response.
 * @param domain buffer to store domain name
 * @param buffer DNS response buffer
 * @param offset offset in buffer where domain name starts
 * @return domain name
 */
char *extractDomain(char *domain, const char *buffer, size_t offset) {
    const char *response = &buffer[offset];
    int i = 0;

    while (*response != 0) {
        if ((*response & 192) != 192) {
            size_t length = *response;
            response++;

            memcpy(&domain[i], response, length);
            i += length;

            response += length;

            if (*response != 0) { domain[i++] = '.'; }
        } else {
            response = &buffer[(*response & 63) << 8 | response[1]];
        }
    }

    domain[i] = '\0';

    return domain;
}

/**
 * Decode domain name from DNS response.
 * @param domain buffer to store domain name
 * @param type DNS record type
 * @param buffer DNS response buffer
 * @param offset offset in buffer where domain name starts
 * @return decoded domain
 */
char *decodeDomain(char *domain, uint16_t type, const char *buffer, size_t offset) {
    if (type == NS_REC || type == PTR_REC || type == CNAME_REC || type == SOA_REC) {
        return extractDomain(domain, buffer, offset + sizeof(dns_response_t));
    }

    char str[INET6_ADDRSTRLEN];

    const void *addressPtr = buffer + offset + sizeof(dns_response_t);

    if (inet_ntop((type == A_REC) ? AF_INET : AF_INET6, addressPtr, str, sizeof(str)) == NULL) {
        perror("inet_ntop failed");
        exit(EXIT_FAILURE);
    }

    strcpy(domain, str);

    return domain;
}

/**
 * Print the DNS section.
 * @param buffer buffer with DNS response
 * @param offset offset in buffer
 * @param count number of records
 * @return offset in buffer
 */
size_t printSection(const char *buffer, size_t offset, uint16_t count) {
    char domain[256];

    for (int i = 0; i < ntohs(count); i++) {
        dns_response_t *response = (dns_response_t *) &buffer[offset];
        memset(domain, 0, sizeof(domain));

        const char *name = &buffer[((response->namePointer & 0xff00) >> 8) + 1];
        const char *dnsType = getDNSType(response->type);
        const char *dnsClass = getDNSClass(response->cls);
        uint32_t ttl = ntohl(response->ttl);

        printf("  %s., %s, %s, %d, %s\n", name, dnsType, dnsClass, ttl,
               decodeDomain(domain, ntohs(response->type), buffer, offset));

        offset += sizeof(dns_response_t) + ntohs(response->dataLength);
    }

    return offset;
}

/**
 * Print the DNS response.
 * @param buffer buffer with DNS response
 * @param domainLen length of domain
 */
void printDnsResponse(char *buffer, size_t domainLen) {
    char domain[256];

    dns_header_t *header = (dns_header_t *) buffer;
    size_t offset = sizeof(dns_header_t) + domainLen;

    processResponseError(ntohs(header->flags) & 15);

    char isAuthoritative = ntohs(header->flags) & AUTH_ANSWER_MASK;
    char isRecursive = ntohs(header->flags) & REC_DESIRED_MASK;
    char isTruncated = ntohs(header->flags) & TRUNCATION_MASK;

    printf("Authoritative: %s, Recursive: %s, Truncated: %s\n", isAuthoritative ? "Yes" : "No",
           isRecursive ? "Yes" : "No", isTruncated ? "Yes" : "No");

    memset(domain, 0, sizeof(domain));
    extractDomain(domain, buffer, DOMAIN_OFFSET_ZERO);
    memcpy(&buffer[DOMAIN_OFFSET], domain, strlen(domain));

    dns_response_t *response = (dns_response_t *) &buffer[offset - sizeof(uint16_t)];
    const char *questionName = (char *) &buffer[sizeof(dns_header_t)] + 1;
    const char *dnsType = getDNSType(response->type);
    const char *dnsClass = getDNSClass(response->cls);

    printf("Question section (%d)\n", ntohs(header->question));
    printf("  %s., %s, %s\n", questionName, dnsType, dnsClass);

    header = (dns_header_t *) buffer;
    offset = sizeof(dns_header_t) + strlen((char *) &buffer[sizeof(dns_header_t)]) + 1 + sizeof(uint16_t) * 2;

    printf("Answer section (%d)\n", ntohs(header->answer));
    offset = printSection(buffer, offset, header->answer);

    printf("Authority section (%d)\n", ntohs(header->authority));
    offset = printSection(buffer, offset, header->authority);

    printf("Additional section (%d)\n", ntohs(header->additional));
    printSection(buffer, offset, header->additional);

    printf("\n");
}
