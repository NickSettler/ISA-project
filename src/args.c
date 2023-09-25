/**
 * @author Nikita Moiseev<xmoise01@stud.fit.vutbr.cz>
 * @file: config.c
 * @date: 18.11.2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>

#include "args.h"
#include "error.h"

/**
 * Parse the command line arguments and set config structure.
 * @param config Pointer to the config structure.
 * @param argc Number of command line arguments.
 * @param argv Array of command line arguments.
 */
void parseArgs(config_t *config, int argc, char *argv[]) {
    config->recursive = 0;
    config->host = NULL;
    config->addr = NULL;
    config->port = 53;
    config->reverseLookup = 0;
    config->reverseIPv6 = 0;
    config->ipv6 = 0;

    int opt;
    while ((opt = getopt(argc, argv, "rx6s:p:")) != -1) {
        switch (opt) {
            case 'r':
                config->recursive = 1;
                break;
            case 'x':
                config->reverseLookup = 1;
                break;
            case '6':
                config->ipv6 = 1;
                break;
            case 's':
                config->host = optarg;
                break;
            case 'p':
                config->port = atoi(optarg);
                break;
            default:
                printUsage();
                ERROR_INVALID_ARGUMENTS
        }
    }

    if (optind < argc) {
        config->addr = argv[optind];
    } else {
        printUsage();
        ERROR_INVALID_ARGUMENTS
    }
}

/**
 * Validate if the given IP address is valid IPv4.
 *
 * @param ip The IP address string to be validated.
 * @return 1 if the IP address is valid IPv4, 0 otherwise.
 */
bool isValidIPv4(const char *ip) {
    struct in_addr addr;
    return inet_pton(AF_INET, ip, &addr) == 1;
}

/**
 * Validate if the given IP address is valid IPv6.
 *
 * @param ip The IP address string to be validated.
 * @return 1 if the IP address is valid IPv6, 0 otherwise.
 */
bool isValidIPv6(const char *ip) {
    struct in6_addr addr;
    return inet_pton(AF_INET6, ip, &addr) == 1;
}

/**
 * Validate if the given domain is valid.
 * @param domain The domain string to be validated.
 * @return 1 if the domain is valid, 0 otherwise.
 */
bool isValidDomain(const char *domain) {
    struct hostent *pHostent = gethostbyname(domain);
    return pHostent != NULL;
}

/**
 * Validate if the given config is valid.
 * @param config The config structure to be validated.
 * @return 0 if the config is valid, 1 otherwise.
 */
void validateArgs(config_t *config) {
    if (config->addr == NULL) {
        fprintf(stderr, "Address must be provided\n");
        ERROR_INVALID_ARGUMENTS;
    }

    if (config->host == NULL) {
        fprintf(stderr, "Host must be provided\n");
        ERROR_INVALID_ARGUMENTS;
    }

    bool isIPv4 = isValidIPv4(config->addr);
    bool isIPv6 = isValidIPv6(config->addr);
    bool isDomain = isValidDomain(config->host);

    if (!isDomain && !isIPv4 && !isIPv6) {
        fprintf(stderr, "Host is not a valid domain or IP address\n");
        ERROR_INVALID_ARGUMENTS;
    }

    if (config->port < 1 || config->port > 65535) {
        fprintf(stderr, "Port must be between 1 and 65535\n");
        ERROR_INVALID_ARGUMENTS;
    }

    if (config->reverseLookup) {
        bool isIPv6Domain = isValidIPv6(config->addr);
        bool isIPv4Domain = isValidIPv4(config->addr);

        if (!(isIPv6Domain || isIPv4Domain)) {
            fprintf(stderr, "Bad combination of address and reverse-lookup flag\n");
            ERROR_INVALID_ARGUMENTS;
        }

        config->reverseIPv6 = isIPv6Domain;
    } else {
        if (!isValidDomain(config->addr)) {
            fprintf(stderr, "Address is not a valid domain\n");
            ERROR_INVALID_ARGUMENTS;
        }
    }
}

/**
 * Print the usage of the program.
 */
void printUsage() { fprintf(stderr, "dns [-r] [-x] [-6] -s host [-p port] address\n"); }
