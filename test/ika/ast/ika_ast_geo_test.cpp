#include "saya_test/ika.hpp"

TEST(Ika_AST_Geo, Root)
{
    auto const res = sayatest::ika_compile(R"(
        $hor-rev+center [12px, 20%] {
            &dummy()
        }
    )");

}
