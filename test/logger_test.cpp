#include "saya_test.hpp"
#include "saya/logger.hpp"
#include "saya/console/color.hpp"

#include <sstream>

#define APP_NAME std::string("Test")

TEST(Logger, NoPrompt)
{
    std::ostringstream oss;
    saya::logger l({}, oss, oss);
    l.color(false);

    EXPECT_TRUE(oss.str().empty());

    l.info() << "Hello" << ", " << "logger!" << std::endl;
    EXPECT_EQ("(INFO) Hello, logger!\n", oss.str());
}

TEST(Logger, SingleLineWithColor)
{
    std::ostringstream oss;
    saya::logger l{APP_NAME, oss, oss};
    // l.color(true);

    l.info() << "Hello" << ", " << "INFO" << std::endl;
    EXPECT_EQ("[" + APP_NAME + "] \x1B[37m(INFO)\x1B[0m " + "Hello, INFO\n", oss.str());
    oss.str({});

    l.warn() << "Hello" << ", " << "WARN" << std::endl;
    EXPECT_EQ("[" + APP_NAME + "] \x1B[43m\x1B[30m(WARN)\x1B[0m " + "Hello, WARN\n", oss.str());
    oss.str({});

    l.error() << "Hello" << ", " << "ERROR" << std::endl;
    EXPECT_EQ("[" + APP_NAME + "] \x1B[41m\x1B[97m(ERROR)\x1B[0m " + "Hello, ERROR\n", oss.str());
    oss.str({});
}


