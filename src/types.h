/**
 * @author Nikita Moiseev<xmoise01@stud.fit.vutbr.cz>
 * @file: types.h
 * @date: 18.11.2023
 */

#ifndef DNS_TYPES_H
#define DNS_TYPES_H

#include <stdio.h>

#define DNS_PACKET_BUFF_SIZE 65536

#define AUTH_ANSWER_MASK 0b00000100
#define TRUNCATION_MASK 0b00000010
#define REC_DESIRED_MASK 0b00000001

#define A_REC 1
#define NS_REC 2
#define CNAME_REC 5
#define SOA_REC 6
#define PTR_REC 12
#define AAAA_REC 28

#define A_REC_TYPE "A"
#define NS_REC_TYPE "NS"
#define CNAME_REC_TYPE "CNAME"
#define SOA_REC_TYPE "SOA"
#define PTR_REC_TYPE "PTR"
#define AAAA_REC_TYPE "AAAA"

#define IN_DNS_CODE 1
#define CS_DNS_CODE 2
#define CH_DNS_CODE 3
#define HS_DNS_CODE 4

#define IN_DNS_TYPE "IN"
#define CS_DNS_TYPE "CS"
#define CH_DNS_TYPE "CH"
#define HS_DNS_TYPE "HS"

#define UNKNOWN_TYPE "UNKNOWN"

#define DOMAIN_OFFSET_ZERO sizeof(dns_header_t)
#define DOMAIN_OFFSET (DOMAIN_OFFSET_ZERO + 1)

/**
 * DNS header structure
 */
typedef struct {
    uint16_t id;
    uint16_t flags;
    uint16_t question;
    uint16_t answer;
    uint16_t authority;
    uint16_t additional;
} dns_header_t;

/**
 * DNS response structure
 */
typedef struct __attribute__((__packed__)) {
    uint16_t namePointer;
    uint16_t type;
    uint16_t cls;
    uint32_t ttl;
    uint16_t dataLength;
} dns_response_t;

#endif// DNS_TYPES_H
