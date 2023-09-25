/**
 * @author Nikita Moiseev<xmoise01@stud.fit.vutbr.cz>
 * @file: request.h
 * @date: 18.11.2023
 */

#ifndef PROJ1_REQUEST_H
#define PROJ1_REQUEST_H

#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "args.h"

void request(int socket, config_t *config);

int openSocket(config_t *config);

size_t writeQueryDomain(char *domain, char *queryName);

size_t writeQueryPtr(config_t *config, char *queryName);

void writeQueryType(config_t *config, unsigned short *buffer);

void sendDNSRequest(int socket, const char *buffer, size_t domainLen);

void getDnsResponse(int socket, char *buffer);

void setHeaders(config_t *config, char *buffer);

char *reverseIP(char *ip);

char *reverseIPv6(char *ipv6Address);

#endif
