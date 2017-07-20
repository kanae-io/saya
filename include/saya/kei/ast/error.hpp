#ifndef SAYA_KEI_AST_ERROR_HPP_
#define SAYA_KEI_AST_ERROR_HPP_

#include "saya/kei_fwd.hpp"

#include "boost/assert.hpp"
#include <stdexcept>

namespace saya { namespace kei { namespace ast {

struct ast_error : std::invalid_argument
{
    ast_error()
        : std::invalid_argument("[BUG] Unknown AST error")
    {
        BOOST_ASSERT(!"[BUG] Unknown AST error");
    }

    explicit ast_error(char const* reason)
        : std::invalid_argument(reason)
    {}

    explicit ast_error(std::string const& reason)
        : std::invalid_argument(reason)
    {}
};

}}} // saya

#endif
