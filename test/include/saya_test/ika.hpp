#ifndef SAYATEST_IKA_HPP
#define SAYATEST_IKA_HPP

#include "saya/ika/grammar/to_ast.hpp"
#include "saya/ika/compiler.hpp"

#include "saya/test.hpp"
#include "saya/logger.hpp"

namespace sayatest {

inline typename saya::ika::grammar::to_ast::root_type
ika_compile(std::string const& buf)
{
    saya::logger l{"ika-test"};

    saya::ika::compiler<saya::ika::grammar::to_ast>
    compiler{&l, saya::ika::compiler_options{
        // TBD
    }};

    compiler.set_buf(&buf);
    return compiler.compile();
}

} // sayatest

#endif
