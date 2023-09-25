/**
 * @author Nikita Moiseev<xmoise01@stud.fit.vutbr.cz>
 * @file: response.h
 * @date: 18.11.2023
 */

#ifndef DNS_RES_H
#define DNS_RES_H

#include <stdio.h>
#include "types.h"
#include "error.h"

char *getDNSType(uint16_t type);

char *getDNSClass(uint16_t cls);

char *extractDomain(char *domain, const char *buffer, size_t offset);

char *decodeDomain(char *domain, uint16_t type, const char *buffer, size_t offset);

void printDnsResponse(char *buffer, size_t domainLen);

size_t printSection(const char *buffer, size_t offset, uint16_t count);

#endif
