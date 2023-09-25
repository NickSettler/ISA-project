#include <string>
#include "test.h"

class ConfigTest : public testing::Test {
protected:
    config_t cfg{};
    char *googleDNSServer = const_cast<char *>((new std::string("8.8.8.8"))->c_str());

    void SetUp() override {
        cfg.recursive = 0;
        cfg.reverseLookup = 0;
        cfg.ipv6 = 0;
        cfg.host = googleDNSServer;
        cfg.port = 0;
        cfg.addr = googleDNSServer;
        cfg.reverseIPv6 = 0;
    }
};

TEST_F(ConfigTest, NullServer) {
    cfg.port = 123;
    cfg.addr = googleDNSServer;
    cfg.host = nullptr;

    EXPECT_EXIT(validateArgs(&cfg), ::testing::ExitedWithCode(ERROR_INVALID_ARGUMENTS_CODE), "Host must be provided");
}

TEST_F(ConfigTest, InvalidPort) {
    cfg.host = googleDNSServer;
    cfg.port = -1;
    cfg.addr = googleDNSServer;

    EXPECT_EXIT(validateArgs(&cfg), ::testing::ExitedWithCode(ERROR_INVALID_ARGUMENTS_CODE),
                "Port must be between 1 and 65535");
}

TEST_F(ConfigTest, NullAddr) {
    cfg.host = googleDNSServer;
    cfg.addr = nullptr;
    cfg.port = 123;

    EXPECT_EXIT(validateArgs(&cfg), ::testing::ExitedWithCode(ERROR_INVALID_ARGUMENTS_CODE),
                "Address must be provided");
}

TEST_F(ConfigTest, InvalidAddr) {
    char *invalidAddr = const_cast<char *>((new std::string("SOMETHInG"))->c_str());

    cfg.host = googleDNSServer;
    cfg.port = 123;
    cfg.addr = invalidAddr;
    cfg.reverseLookup = 1;
    
    EXPECT_EXIT(validateArgs(&cfg), ::testing::ExitedWithCode(ERROR_INVALID_ARGUMENTS_CODE),
                "Bad combination of address and reverse-lookup flag");
}
