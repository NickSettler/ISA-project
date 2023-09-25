#include <netinet/in.h>
#include "test.h"

TEST(DNSRequestTest, ReverseIPv4) {
    char actual[INET_ADDRSTRLEN] = "192.168.0.1";

    const char *expected = "1.0.168.192";

    EXPECT_STREQ(expected, reverseIP(actual));
}

TEST(DNSRequestTest, ReverseIPv6) {
    char actual[INET6_ADDRSTRLEN * 2] = "2023:4860:4860::1234";
    const char *expected = "\u00014\u00013\u00012\u00011"
                           "\u00010\u00010\u00010\u00010"
                           "\u00010\u00010\u00010\u00010"
                           "\u00010\u00010\u00010\u00010"
                           "\u00010\u00010\u00010\u00010"
                           "\u00010\u00016\u00018\u00014"
                           "\u00010\u00016\u00018\u00014"
                           "\u00013\u00012\u00010\u00012"
                           "\u0003ip6\u0004arpa";

    EXPECT_STREQ(expected, reverseIPv6(actual));
}

TEST(DNSRequestTest, WriteQueryDomain) {
    char buffer[1000];
    char domain[50] = "www.google.com";

    memset(buffer, 0, sizeof(buffer));
    const char *expected = "\x3www\x6google\3com\0";

    size_t queryLength = writeQueryDomain(domain, buffer);

    EXPECT_STREQ(expected, (char *) buffer);
    EXPECT_EQ(strlen(expected), queryLength - 1);
}

TEST(DNSRequestTest, QueryType) {
    config_t config;
    unsigned short buffer[2];

    config.reverseLookup = false;
    config.ipv6 = false;
    writeQueryType(&config, buffer);
    EXPECT_EQ(htons(A_REC), buffer[0]);
    EXPECT_EQ(htons(1), buffer[1]);
}

TEST(DNSRequestTest, SetHeaders) {
    char buffer[sizeof(dns_header_t)]{};
    config_t config{};

    memset(buffer, 0, sizeof(dns_header_t));
    config.recursive = 1;

    setHeaders(&config, buffer);

    auto *dnsHeader = (dns_header_t *) buffer;

    EXPECT_EQ(dnsHeader->id, htons(getpid()));
    EXPECT_EQ(dnsHeader->flags, config.recursive);
    EXPECT_EQ(ntohs(dnsHeader->question), 1);
    EXPECT_EQ(dnsHeader->answer, 0);
    EXPECT_EQ(dnsHeader->authority, 0);
    EXPECT_EQ(dnsHeader->additional, 0);
}
