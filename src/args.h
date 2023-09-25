/**
 * @author Nikita Moiseev<xmoise01@stud.fit.vutbr.cz>
 * @file: config.h
 * @date: 18.11.2023
 */

#ifndef ISA_CONFIG_H
#define ISA_CONFIG_H

#include <stdint.h>
#include <stdbool.h>

/**
 * Configuration structure.
 *
 * @property recursive Recursive flag.
 * @property host Hostname to be resolved.
 * @property addr IP address to be resolved.
 * @property port Port number.
 * @property reverseLookup Reverse lookup flag.
 * @property reverseIPv6 Reverse IPv6 flag.
 * @property ipv6 IPv6 flag.
 */
typedef struct config_struct {
    uint16_t recursive;

    char *host;
    char *addr;
    int port;

    int reverseLookup;
    int reverseIPv6;
    int ipv6;
} config_t;

void parseArgs(config_t *config, int argc, char *argv[]);

void validateArgs(config_t *config);

void printUsage();

#endif
