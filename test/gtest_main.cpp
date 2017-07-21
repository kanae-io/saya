#include "saya_test.hpp"
#include "saya_test/bridge.hpp"

GTEST_API_ int main(int argc, char* argv[])
{
    saya_test::bridge().init(argc, argv);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

