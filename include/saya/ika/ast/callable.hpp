#ifndef SAYA_IKA_AST_CALLABLE_HPP
#define SAYA_IKA_AST_CALLABLE_HPP

#include "saya/ika/ast_fwd.hpp"
#include "saya/ika/ast/ast_entity.hpp"
#include "saya/ika/ast/geo.hpp"
#include "saya/ika/ast/block.hpp"

#include <boost/assert.hpp>

#include <vector>

namespace saya { namespace ika { namespace ast {

struct Func : ASTEntity
{
    FuncID id;
    Block* definition{nullptr};

    explicit Func(FuncID const& id)
        : id(id)
    {}

    void operator[](Block* v)
    {
        BOOST_ASSERT(v);
        definition = v;
    }
};

struct Macro : ASTEntity
{
    MacroID id;
    Geo* definition{nullptr};

    explicit Macro(MacroID const& id)
        : id(id)
    {}

    void operator[](Geo* v)
    {
        BOOST_ASSERT(v);
        definition = v;
    }
};

struct CallParam : ASTEntity
{
    std::vector<Expr> expr_list;
};

}}} // saya

#endif
