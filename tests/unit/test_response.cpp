#include "test.h"

TEST(DNSResponseTest, DNSClass) {
    EXPECT_EQ(strcmp(getDNSClass(htons(IN_DNS_CODE)), IN_DNS_TYPE), 0);
    EXPECT_EQ(strcmp(getDNSClass(htons(CS_DNS_CODE)), CS_DNS_TYPE), 0);
    EXPECT_EQ(strcmp(getDNSClass(htons(CH_DNS_CODE)), CH_DNS_TYPE), 0);
    EXPECT_EQ(strcmp(getDNSClass(htons(HS_DNS_CODE)), HS_DNS_TYPE), 0);
    EXPECT_EQ(strcmp(getDNSClass(htons(0)), UNKNOWN_TYPE), 0);
}

TEST(DNSResponseTest, DNSType) {
    EXPECT_EQ(strcmp(getDNSType(htons(A_REC)), A_REC_TYPE), 0);
    EXPECT_EQ(strcmp(getDNSType(htons(NS_REC)), NS_REC_TYPE), 0);
    EXPECT_EQ(strcmp(getDNSType(htons(CNAME_REC)), CNAME_REC_TYPE), 0);
    EXPECT_EQ(strcmp(getDNSType(htons(SOA_REC)), SOA_REC_TYPE), 0);
    EXPECT_EQ(strcmp(getDNSType(htons(PTR_REC)), PTR_REC_TYPE), 0);
    EXPECT_EQ(strcmp(getDNSType(htons(AAAA_REC)), AAAA_REC_TYPE), 0);
    EXPECT_EQ(strcmp(getDNSType(htons(0)), UNKNOWN_TYPE), 0);
}

TEST(DNSResponseTest, ExtractDomain) {
    char buf[] = {0x03, 'w', 'w', 'w', 0x06, 'g', 'o', 'o', 'g', 'l', 'e', 0x03, 'c', 'o', 'm', 0x00};
    char domain[256] = {0};

    extractDomain(domain, buf, 0);

    EXPECT_STRCASEEQ(domain, "www.google.com");
}

TEST(DNSResponseTest, A_RECORD) {
    char domain[INET_ADDRSTRLEN];
    char buf[100];

    const char *ipAddress = "8.8.8.8";
    inet_pton(AF_INET, ipAddress, buf + sizeof(dns_response_t));

    EXPECT_STREQ(decodeDomain(domain, A_REC, buf, 0), ipAddress);
}

TEST(DNSResponseTest, AAAA_RECORD) {
    char domain[INET6_ADDRSTRLEN];
    char buf[100];

    const char *ipAddress = "2023:ac8:15b3::7f11:430:7129";
    inet_pton(AF_INET6, ipAddress, buf + sizeof(dns_response_t));

    EXPECT_STREQ(decodeDomain(domain, AAAA_REC, buf, 0), ipAddress);
}

TEST(DNSResponseTest, NS_RECORD) {
    char domain[INET6_ADDRSTRLEN];
    char buf[100];

    memset(buf, 0, sizeof(buf));
    memset(domain, 0, sizeof(domain));

    const char *ipAddress = "google.com";
    strcpy(buf + sizeof(dns_response_t), "\U00000006google\U00000003com\0");

    EXPECT_STREQ(decodeDomain(domain, NS_REC, buf, 0), ipAddress);
}
