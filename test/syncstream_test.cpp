#include "saya_test.hpp"
#include "saya/syncstream.hpp"

#include <sstream>

TEST(SyncStream, TempObject)
{
    std::ostringstream oss;
    saya::osyncstream(oss) << "Hello" << ", " << "world!" << std::flush;
    EXPECT_EQ("Hello, world!", oss.str());
}

TEST(SyncStream, NoFlush)
{
    std::ostringstream oss;
    saya::osyncstream(oss) << "Hello" << ", " << "world!";
    EXPECT_EQ(13, oss.tellp());
    EXPECT_EQ("Hello, world!", oss.str());
}

TEST(SyncStream, EmitOnDestructorWithFlush)
{
    std::ostringstream oss;
    {
        saya::osyncstream ss(oss);
        ss << "Hello" << ", " << "world!" << std::flush;

        EXPECT_TRUE(oss.str().empty());
    }
    EXPECT_EQ("Hello, world!", oss.str());
}

TEST(SyncStream, EmitOnDestructorWithoutFlush)
{
    std::ostringstream oss;
    {
        saya::osyncstream ss(oss);
        ss << "Hello" << ", " << "world!";

        EXPECT_TRUE(oss.str().empty());
    }
    EXPECT_EQ("Hello, world!", oss.str());
}

TEST(SyncStream, WildFlush)
{
    std::ostringstream oss;
    {
        saya::osyncstream ss(oss);
        ss << "Hello" << ", " << "world!";

        EXPECT_TRUE(oss.str().empty());

        ss << std::flush;
        EXPECT_TRUE(oss.str().empty());

        ss << std::flush;
        EXPECT_TRUE(oss.str().empty());
    }
    EXPECT_EQ("Hello, world!", oss.str());
}

TEST(SyncStream, ExplicitEmit)
{
    std::ostringstream oss;
    EXPECT_NO_THROW({
        saya::osyncstream ss(oss);
        ss << "Hello" << ", " << "world!";

        EXPECT_TRUE(oss.str().empty());

        EXPECT_NO_THROW(ss.emit());
        EXPECT_EQ("Hello, world!", oss.str());

        EXPECT_NO_THROW(ss.emit());
        EXPECT_EQ("Hello, world!", oss.str());

        ss << "Second" << " " << "try";
        EXPECT_EQ("Hello, world!", oss.str());

        ss << std::flush;
        EXPECT_EQ("Hello, world!", oss.str());

        EXPECT_NO_THROW(ss.emit());
        EXPECT_EQ("Hello, world!Second try", oss.str());

        ss << "END";
    });
    EXPECT_EQ("Hello, world!Second tryEND", oss.str());
}

TEST(SyncStream, NestedEasy)
{
    std::ostringstream oss;
    saya::osyncstream ss0(oss);
    ss0 << "0";
    EXPECT_TRUE(oss.str().empty());

    {
        saya::osyncstream ss1(oss);
        ss1 << "1";
        EXPECT_TRUE(oss.str().empty());

        {
            saya::osyncstream ss2(oss);
            ss2 << "2";
            EXPECT_TRUE(oss.str().empty());
        }
        EXPECT_EQ("2", oss.str());

        saya::osyncstream ss3(oss);
        ss3 << "3";
        EXPECT_EQ("2", oss.str());

        {
            saya::osyncstream ss4(oss);
            ss4 << "4";
            EXPECT_EQ("2", oss.str());

            {
                saya::osyncstream ss5(oss);
                ss5 << "5";
                EXPECT_EQ("2", oss.str());
            }
            EXPECT_EQ("25", oss.str());

            saya::osyncstream ss6(oss);
            ss6 << "6";
            EXPECT_EQ("25", oss.str());
        }
        EXPECT_EQ("2564", oss.str());

        saya::osyncstream ss7(oss);
        ss7 << "7";
        EXPECT_EQ("2564", oss.str());
    }
    EXPECT_EQ("2564731", oss.str());

    saya::osyncstream ss8(oss);
    ss8 << "8";
    EXPECT_EQ("2564731", oss.str());
}

TEST(SyncStream, Nested)
{
    std::ostringstream oss;
    {
        saya::osyncstream ss(oss);
        ss << "First" << " " << "try";
        EXPECT_TRUE(oss.str().empty());

        {
            // Should steal "First try" from `ss` due to std::move()
            saya::osyncstream ss2(std::move(ss));
            ss2 << "Second" << " " << "try";
            EXPECT_TRUE(oss.str().empty());

            {
                saya::osyncstream ss3(oss.rdbuf());
                ss3 << "Third" << " " << "try";
                EXPECT_TRUE(oss.str().empty());
            }
            EXPECT_EQ("Third try", oss.str());
        }
        EXPECT_EQ("Third tryFirst trySecond try", oss.str());
    }
    EXPECT_EQ("Third tryFirst trySecond try", oss.str());
}

