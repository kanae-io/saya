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
    oss.str({});
}

TEST(Logger, NoteShouldIndentWell)
{
    std::ostringstream oss;
    saya::logger l(APP_NAME, oss, oss);
    l.color(false);

    l.note() << "1" << std::endl;
    l.info() << "test1" << std::endl;
    EXPECT_EQ("[Test] (INFO)  test1\n         note: 1\n", oss.str());
    oss.str({});

    l.note() << "2" << std::endl;
    l.warn() << "test2" << std::endl;
    EXPECT_EQ("[Test] (WARN)  test2\n         note: 2\n", oss.str());
    oss.str({});

    l.note() << "3" << std::endl;
    l.error() << "test3" << std::endl;
    EXPECT_EQ("[Test] (ERROR) test3\n         note: 3\n", oss.str());
    oss.str({});
}

TEST(Logger, SingleLineWithColor)
{
    std::ostringstream oss;
    saya::logger l{APP_NAME, oss, oss};
    // l.color(true);

    l.info() << "Hello" << ", " << "INFO" << std::endl;
    EXPECT_EQ("\x1B[34m[" + APP_NAME + "]\x1B[0m \x1B[100m\x1B[97mINFO\x1B[0m  " + "Hello, INFO\n", oss.str());
    oss.str({});

    l.warn() << "Hello" << ", " << "WARN" << std::endl;
    EXPECT_EQ("\x1B[34m[" + APP_NAME + "]\x1B[0m \x1B[43m\x1B[30mWARN\x1B[0m  " + "Hello, WARN\n", oss.str());
    oss.str({});

    l.error() << "Hello" << ", " << "ERROR" << std::endl;
    EXPECT_EQ("\x1B[34m[" + APP_NAME + "]\x1B[0m \x1B[41m\x1B[97mERROR\x1B[0m " + "Hello, ERROR\n", oss.str());
    oss.str({});
}


