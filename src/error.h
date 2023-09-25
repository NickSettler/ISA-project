/**
 * @author Nikita Moiseev<xmoise01@stud.fit.vutbr.cz>
 * @file: error.h
 * @date: 18.11.2023
 */

#ifndef DNS_ERROR_H
#define DNS_ERROR_H

#include <stdio.h>
#include <stdlib.h>

#define ERROR_INVALID_ARGUMENTS_CODE 1

#define ERROR_DNS_RESPONSE_FORMAT_CODE 11
#define ERROR_DNS_RESPONSE_SERVER_FAILURE_CODE 12
#define ERROR_DNS_RESPONSE_NX_DOMAIN_CODE 13
#define ERROR_DNS_RESPONSE_NOT_IMP_CODE 14
#define ERROR_DNS_RESPONSE_REFUSED_CODE 15
#define ERROR_DNS_RESPONSE_X_DOMAIN_CODE 16
#define ERROR_DNS_RESPONSE_X_RR_SET_CODE 17
#define ERROR_DNS_RESPONSE_NOT_AUTH_CODE 18
#define ERROR_DNS_RESPONSE_NOT_ZONE_CODE 19

#define ERROR(msg, code)                                                                                               \
    fprintf(stderr, "%s\n", msg);                                                                                      \
    exit(code);

#define ERROR_INVALID_ARGUMENTS ERROR("Invalid arguments.", ERROR_INVALID_ARGUMENTS_CODE)

#define ERROR_DNS_RESPONSE_FORMAT ERROR("DNS response format error.", ERROR_DNS_RESPONSE_FORMAT_CODE)

#define ERROR_DNS_RESPONSE_SERVER_FAILURE ERROR("DNS response server failure.", ERROR_DNS_RESPONSE_SERVER_FAILURE_CODE)

#define ERROR_DNS_RESPONSE_NX_DOMAIN ERROR("DNS response host not found.", ERROR_DNS_RESPONSE_NX_DOMAIN_CODE)

#define ERROR_DNS_RESPONSE_NOT_IMP ERROR("DNS response not implemented.", ERROR_DNS_RESPONSE_NOT_IMP_CODE)

#define ERROR_DNS_RESPONSE_REFUSED ERROR("DNS response refused.", ERROR_DNS_RESPONSE_REFUSED_CODE)

#define ERROR_DNS_RESPONSE_X_DOMAIN ERROR("DNS response domain name is not exist.", ERROR_DNS_RESPONSE_X_DOMAIN_CODE)

#define ERROR_DNS_RESPONSE_X_RR_SET ERROR("DNS response RR set is not exist.", ERROR_DNS_RESPONSE_X_RR_SET_CODE)
 
#define ERROR_DNS_RESPONSE_NOT_AUTH ERROR("DNS response not authorized.", ERROR_DNS_RESPONSE_NOT_AUTH_CODE)

#define ERROR_DNS_RESPONSE_NOT_ZONE ERROR("DNS response not zone.", ERROR_DNS_RESPONSE_NOT_ZONE_CODE)

void processResponseError(int errorCode);

#endif// DNS_ERROR_H
